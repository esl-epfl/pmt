#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <filesystem>
#include <cerrno>
#include <system_error>

#include <sched.h>
#include <unistd.h>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <charconv>

#include "RaplImpl.h"
#include "fmt/base.h"

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
  return fmt::format("/sys/class/powercap/intel-rapl:{0}", package_id);
}

std::string read_string(int fd) {
  std::string result;
  std::array<std::byte, 256> buffer;
  const std::size_t read_size = os::pread(fd, buffer, 0);
  result.reserve(read_size);
  for (std::size_t i = 0; i < read_size; ++i) {
    const char c = static_cast<char>(buffer[i]);
    if (!std::isspace(c) && c != '\n') {
      result.push_back(c);
    }
  }
  return result;
}

template <typename Numeric>
Numeric read_numerical_value(int fd) {
  Numeric value = 0;
  std::array<std::byte, 256> buffer;
  const std::size_t read_size = os::pread(fd, buffer, 0);
  std::from_chars_result res = std::from_chars(
      static_cast<const char*>(static_cast<void*>(buffer.data())),
      static_cast<const char*>(static_cast<void*>(buffer.data() + read_size)),
      value);
  if (res.ec == std::errc::invalid_argument &&
      res.ec == std::errc::result_out_of_range) {
    throw std::runtime_error("Unable to parse file to numerical value");
  }
  return value;
}

void RaplImpl::Init() {
  try {
    for (std::size_t i = 0; i < kNumRaplDomains; i++) {
      try {
        std::string basename = GetBasename(i);

        if (!std::filesystem::exists(basename)) {
          break;
        }

        const os::file_descriptor fd_rapl_dir = os::opendir(basename);
        const os::file_descriptor name_fd =
            os::openat(fd_rapl_dir.fd(), "name");

        // Read domain name
        const std::string package_id = read_string(name_fd.fd());

        const os::file_descriptor max_energy_fd =
            os::openat(fd_rapl_dir.fd(), "max_energy_range_uj");
        std::size_t max_energy_range_uj =
            read_numerical_value<std::size_t>(max_energy_fd.fd());

        os::file_descriptor energy_uj_fd =
            os::openat(fd_rapl_dir.fd(), "energy_uj");
        (void)read_numerical_value<std::size_t>(energy_uj_fd.fd());

        packages_names_.push_back(package_id);
        uj_max_.push_back(max_energy_range_uj);
        energy_fds_.emplace_back(std::move(energy_uj_fd));
      } catch (std::system_error& e) {
        fmt::print(stderr, "OS error: {0}\n", e.what());
        if (e.code().value() == EACCES) {
          fmt::print(stderr,
                     "Please check the permission or try to run as 'root'\n");
        }
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

  } catch (std::exception& e) {
    fmt::print(stderr, "Unable to init rapl plugin: {0}", e.what());
  }
}

std::vector<RaplMeasurement> RaplImpl::GetMeasurements() {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<RaplMeasurement> measurements;

  const std::size_t n = packages_names_.size();
  assert(n == energy_fds_.size());
  assert(n == uj_max_.size());

  auto energy_fd_it = energy_fds_.begin();
  auto packages_name_it = packages_names_.begin();

  // Take all measurements
  while (energy_fd_it < energy_fds_.end()) {
    std::size_t measurement;
    const std::size_t energy_value =
        read_numerical_value<std::size_t>(energy_fd_it->fd());
    measurements.emplace_back(RaplMeasurement{*packages_name_it, energy_value});
    energy_fd_it++;
    packages_name_it++;
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
