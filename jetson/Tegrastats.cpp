#include "Tegrastats.h"

#include <array>
#include <chrono>
#include <thread>
#include <memory>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <vector>

namespace detail {
bool file_exists(const std::string& name) {
  if (FILE* file = fopen(name.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}

std::string exec(const std::string& commandline) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commandline.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

std::string start_tegrastats(int interval) {
  char filename[32] = "/tmp/tegrastats-XXXXXX";
  if (mkstemp(filename) == -1) {
    throw std::runtime_error("Could not create temporary file");
  }
  const char* binary = "/usr/bin/tegrastats";
  std::stringstream commandline;
  commandline << binary << " --start --interval " << interval << " --logfile "
              << filename;
  std::string output = exec(commandline.str());
  if (output.compare("") != 0) {
    throw std::runtime_error("Error starting tegrastats: " + output);
  }
  return filename;
}

void stop_tegrastats(const std::string& logfile) {
  if (logfile.compare("") != 0) {
    const char* binary = "/usr/bin/tegrastats";
    std::stringstream commandline;
    commandline << binary << " --stop";
    exec(commandline.str());
    std::remove(logfile.c_str());
  }
}

std::string read_last_line(const std::string filename) {
  std::stringstream commandline;
  commandline << "/usr/bin/tail -n 1 " << filename;
  return exec(commandline.str());
}

std::vector<std::pair<std::string, int>> read_power_measurements(
    const std::string& line) {
  std::vector<std::pair<std::string, int>> measurements;

  // Regular expression to find individual power measurements
  std::regex regex("\\w+ \\d+mW\\/\\d+mW");
  auto begin = std::sregex_iterator(line.begin(), line.end(), regex);
  auto end = std::sregex_iterator();

  for (std::sregex_iterator match = begin; match != end; ++match) {
    // Get the name
    std::string entry = match->str();
    std::smatch m;
    std::regex_search(entry, m, std::regex("\\w+"));
    std::string name = *m.begin();

    // The name is followed by XXXmw/YYYmw, with XXX being the current power
    // consumption and YYY the average power consumption. This code only
    // extracts the current power consumption.
    std::regex_search(entry, m, std::regex(" (\\d+)"));
    const int value = atoi(m[1].str().c_str());

    // Store the result
    measurements.emplace_back(name, value);
  }

  return measurements;
}
}  // end namespace detail

#include <iostream>
namespace tegrastats {
Tegrastats::Tegrastats() {
  if (detail::file_exists(logfile)) {
    throw std::runtime_error("tegrastats is running, but log file " + logfile +
                             " could not be read.");
  } else {
    detail::stop_tegrastats(logfile);
    logfile = detail::start_tegrastats(interval);
    started_tegrastats = true;
  }
}

Tegrastats::~Tegrastats() {
  if (started_tegrastats) {
    detail::stop_tegrastats(logfile);
    started_tegrastats = false;
  }
}

std::vector<std::pair<std::string, int>> Tegrastats::measure() {
  std::string line = detail::read_last_line(logfile);

  while (line.compare("") == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    line = detail::read_last_line(logfile);
  }

  return detail::read_power_measurements(line);
};

}  // end namespace tegrastats