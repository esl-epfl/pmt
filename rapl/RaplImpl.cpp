#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <cerrno>
#include <system_error>

#include <errno.h>
#include <ext/alloc_traits.h>
#include <sched.h>
#include <unistd.h>

#include "RaplImpl.h"

/*
 * RAPL (Running Average Power Limit) is an API provided by Intel for power
 * monitoring on Intel hardware. It allows access to energy consumption data for
 * each socket and its components.
 *
 * The API provides:
 * - Total energy consumption per socket:
 *   /sys/class/powercap/intel-rapl:X/energy_uj
 *
 * For each socket, it also breaks down the energy consumption into:
 * - Core energy:
 *   /sys/class/powercap/intel-rapl:X:0
 * - Non-core energy:
 *   /sys/class/powercap/intel-rapl:X:1
 */

namespace pmt::rapl {

RaplImpl::RaplImpl() {
  Init();
  previous_timestamp_ = GetTime();
  previous_measurements_ = GetMeasurements();
}

std::string GetBasename(int package_id) {
  return std::string("/sys/class/powercap/intel-rapl:" +
                     std::to_string(package_id));
}

template <typename T>
bool ReadFile(const std::string& filename, T& destination) {
  T temp1;
  T temp2;
  while (true) {
    std::ifstream file(filename);
    if (!file.is_open()) {
#if defined(DEBUG)
      std::cerr << "Could not open " << filename << std::endl;
#endif
      return false;
    } else {
      file >> temp1;
      file.seekg(0);
      file >> temp2;
      if (temp1 == temp2) {
        destination = temp1;
        return true;
      }
    }
  }
}

void RaplImpl::Init() {
  for (std::size_t i = 0; i < kNumRaplDomains; i++) {
    std::string basename = GetBasename(i);

    // Read domain name
    const std::string filename_domain = basename + "/name";
    std::string package_id;

    bool valid = ReadFile(filename_domain, package_id);

    // Read max energy
    const std::string filename_max_energy = basename + "/max_energy_range_uj";
    std::size_t max_energy_range_uj = 0;
    if (valid) {
      valid &= ReadFile(filename_max_energy, max_energy_range_uj);
    }

    // Read energy
    const std::string filename_energy = basename + "/energy_uj";

    // test access right
    const int access_ret_code = ::access(filename_energy.c_str(), R_OK);
    if (valid && access_ret_code != 0) {
      std::ostringstream ss;
      ss << "Unable to access '" << filename_domain
         << "' : " << ::strerror(errno) << "\n"
         << "Please check the permission or try to run as 'root'";
      throw std::runtime_error(ss.str());
    }

    if (valid) {
      std::size_t energy_uj = 0;
      valid &= ReadFile(filename_energy, energy_uj);
    }

    if (valid) {
      packages_names_.push_back(package_id);
      uj_max_.push_back(max_energy_range_uj);
      file_names_.push_back(filename_energy);
    }
  }

  // Initialize state variables
  const std::size_t n = uj_max_.size();
  uj_first_.resize(n);
  uj_previous_.resize(n);
  uj_offset_.resize(n);
  std::vector<RaplMeasurement> measurements = GetMeasurements();
  for (std::size_t i = 0; i < n; i++) {
    uj_first_[i] = measurements[i].joules;
    uj_previous_[i] = uj_first_[i];
    uj_offset_[i] = 0;
  }
}

std::vector<RaplMeasurement> RaplImpl::GetMeasurements() {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<RaplMeasurement> measurements;

  const std::size_t n = packages_names_.size();
  assert(n == file_names_.size());
  assert(n == uj_max_.size());

  auto file_name = file_names_.begin();
  auto packages_name = packages_names_.begin();

  // Take all measurements
  for (std::size_t i = 0; i < n; i++) {
    std::size_t measurement;
    if (!ReadFile(*file_name, measurement)) {
      throw std::runtime_error("Could not open " + *file_name);
    }
    measurements.push_back({*packages_name, measurement});

    file_name = std::next(file_name);
    packages_name = std::next(packages_name);
  }

  for (std::size_t i = 0; i < measurements.size(); i++) {
    const std::size_t uj_now = measurements[i].joules;
    if (uj_now < uj_previous_[i]) {
      uj_offset_[i] += uj_max_[i];
    }
    uj_previous_[i] = uj_now;
    measurements[i].joules = uj_offset_[i] + uj_now - uj_first_[i];
  }

  return measurements;
}  // end Rapl::GetMeasurement

State RaplImpl::GetState() {
  std::vector<RaplMeasurement> measurements = GetMeasurements();
  State state(1 + measurements.size());
  state.timestamp_ = GetTime();
  state.name_[0] = "total";
  state.joules_[0] = 0;
  state.watt_[0] = 0;

  for (std::size_t i = 0; i < measurements.size(); i++) {
    const std::string name = measurements[i].name;
    const std::size_t joules_now = measurements[i].joules;
    const std::size_t joules_previous = previous_measurements_[i].joules;
    const double duration = state.timestamp_ - previous_timestamp_;
    const float joules_diff = (joules_now - joules_previous) * 1e-6;
    const float watt = joules_diff / duration;
    state.name_[i + 1] = name;
    state.joules_[i + 1] = joules_now * 1e-6;
    state.watt_[i + 1] = watt;

    if (name.find("package") != std::string::npos) {
      state.joules_[0] += joules_now * 1e-6;
      state.watt_[0] += watt;
    }
  }

  previous_timestamp_ = state.timestamp_;
  previous_measurements_ = measurements;

  return state;
}

}  // namespace pmt::rapl
