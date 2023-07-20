#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <unistd.h>

#include "RaplImpl.h"

namespace {
std::vector<int> GetActiveCPUs() {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);

  const int result = sched_getaffinity(0, sizeof(cpu_set), &cpu_set);
  if (result == -1) {
    throw std::system_error(errno, std::generic_category(),
                            "sched_getaffinity");
  }

  const int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  std::vector<int> active_cpus;
  for (int cpu = 0; cpu < n_cpus; ++cpu) {
    if (CPU_ISSET(cpu, &cpu_set)) {
      active_cpus.push_back(cpu);
    }
  }

  return active_cpus;
}

template <typename T>
void RemoveDuplicates(std::vector<T>& v) {
  std::sort(v.begin(), v.end());
  v.erase(std::unique(v.begin(), v.end()), v.end());
}
}  // namespace

namespace pmt::rapl {

RaplImpl::RaplImpl() {
  Init();
  previous_timestamp_ = GetTime();
  previous_measurements_ = GetMeasurements();
}

std::vector<int> RaplImpl::DetectPackages() {
  std::string filename;
  std::ifstream file;

  const int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);

  std::vector<int> active_cpus = GetActiveCPUs();
  std::vector<int> active_packages;

  for (int cpu = 0; cpu < n_cpus; cpu++) {
    filename = "/sys/devices/system/cpu/cpu" + std::to_string(cpu) +
               "/topology/physical_package_id";
    file.open(filename);
    if (!file.is_open()) {
      break;
    }
    int package;
    file >> package;
    file.close();

    const bool cpu_is_active = std::find(active_cpus.begin(), active_cpus.end(),
                                         cpu) != active_cpus.end();
    if (cpu_is_active) {
      active_packages.push_back(package);
    }
  }

  RemoveDuplicates(active_packages);

#if defined(DEBUG)
  std::cout << "Detected " << active_cpus.size() << " cores in "
            << active_packages.size() << " package";
  if (active_packages.size() > 1) {
    std::cout << "s";
  }
  std::cout << std::endl;
#endif

  return active_packages;
}

std::string GetBasename(int package_id) {
  return std::string("/sys/class/powercap/intel-rapl/intel-rapl:" +
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
  // Get active packages
  const std::vector<int> package_ids = DetectPackages();

  for (int package_id : package_ids) {
    for (int domain = 0; domain < kNumRaplDomains; domain++) {
      std::string basename = GetBasename(package_id);
      if (domain > 0) {
        basename += "/intel-rapl:" + std::to_string(package_id) + ":" +
                    std::to_string(domain - 1);
      }

      // Read domain name
      const std::string filename_domain = basename + "/name";
      std::string domain_name;
      bool valid = ReadFile(filename_domain, domain_name);
      if (domain > 0) {
        domain_name = domain_name + "-" + std::to_string(package_id);
      }

      // Read max energy
      const std::string filename_max_energy = basename + "/max_energy_range_uj";
      size_t max_energy_range_uj = 0;
      if (valid) {
        valid &= ReadFile(filename_max_energy, max_energy_range_uj);
      }

      // Read energy
      const std::string filename_energy = basename + "/energy_uj";
      if (valid) {
        size_t energy_uj = 0;
        valid &= ReadFile(filename_energy, energy_uj);
      }

      if (valid) {
        domain_names_.push_back(domain_name);
        uj_max_.push_back(max_energy_range_uj);
        file_names_.push_back(filename_energy);
      }
    }
  }

  // Initialize state variables
  const size_t n = uj_max_.size();
  uj_first_.resize(n);
  uj_previous_.resize(n);
  uj_offset_.resize(n);
  std::vector<RaplMeasurement> measurements = GetMeasurements();
  for (size_t i = 0; i < n; i++) {
    uj_first_[i] = measurements[i].joules;
    uj_previous_[i] = uj_first_[i];
    uj_offset_[i] = 0;
  }
}

std::vector<RaplMeasurement> RaplImpl::GetMeasurements() {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<RaplMeasurement> measurements;

  const size_t n = domain_names_.size();
  assert(n == file_names_.size());
  assert(n == uj_max_.size());

  auto file_name = file_names_.begin();
  auto domain_name = domain_names_.begin();

  // Take all measurements
  for (size_t i = 0; i < n; i++) {
    size_t measurement;
    if (!ReadFile(*file_name, measurement)) {
      throw std::runtime_error("Could not open " + *file_name);
    }
    measurements.push_back({*domain_name, measurement});

    file_name = std::next(file_name);
    domain_name = std::next(domain_name);
  }

  for (size_t i = 0; i < measurements.size(); i++) {
    const size_t uj_now = measurements[i].joules;
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

  for (size_t i = 0; i < measurements.size(); i++) {
    const std::string name = measurements[i].name;
    const size_t joules_now = measurements[i].joules;
    const size_t joules_previous = previous_measurements_[i].joules;
    const double duration = state.timestamp_ - previous_timestamp_;
    const size_t joules_diff = (joules_now - joules_previous) * 1e-6;
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
