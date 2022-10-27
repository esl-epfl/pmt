#include <fstream>
#include <iostream>

#include <omp.h>
#include <unistd.h>

#include "Jetson.h"
#include "Tegrastats.h"

namespace pmt {
namespace jetson {

class Jetson_ : public Jetson {
 public:
  Jetson_();

 private:
  class JetsonState {
   public:
    operator State();
    double timeAtRead;
    std::vector<std::pair<std::string, int>> measurements;
    unsigned int instantaneousPowerTotal = 0;
    unsigned int consumedEnergyTotal = 0;
  };

  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/Jetson.out"; }

  virtual int getDumpInterval() {
    return 100;  // milliseconds
  }

  tegrastats::Tegrastats tegrastats;
  JetsonState previousState;
  JetsonState read_jetson();
};

Jetson_::JetsonState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyTotal * 1e-3;

  state.misc.reserve(measurements.size());
  for (auto &measurement : measurements) {
    state.misc.push_back(measurement.second);
  }

  return state;
}

std::unique_ptr<Jetson> Jetson::create() {
  return std::unique_ptr<Jetson>(new Jetson_());
}

Jetson_::Jetson_() {
  previousState = read_jetson();
  previousState.consumedEnergyTotal = 0;
}

Jetson_::JetsonState Jetson_::read_jetson() {
  JetsonState state;
  state.timeAtRead = get_wtime();
  state.measurements = tegrastats.measure();

  // Compute total power consumption as sum of SOC (CPU + DRAM) and GPU.
  // Which individual measurements to use differ per platform.
  state.instantaneousPowerTotal = 0;
  if (state.measurements.size() == 6 &&
      state.measurements[0].first.compare("GPU") == 0 &&
      state.measurements[2].first.compare("SOC") == 0) {
    // AGX Xavier
    state.instantaneousPowerTotal =
        state.measurements[0].second + state.measurements[2].second;
  } else if (state.measurements.size() == 6 &&
             state.measurements[0].first.compare("VDD_GPU_SOC") == 0 &&
             state.measurements[1].first.compare("VDD_CPU_CV") == 0) {
    // AGX Orin
    state.instantaneousPowerTotal =
        state.measurements[0].second + state.measurements[1].second;
  }

  state.consumedEnergyTotal = previousState.consumedEnergyTotal;
  float averagePower =
      (state.instantaneousPowerTotal + previousState.instantaneousPowerTotal) /
      2;
  float timeElapsed = (state.timeAtRead - previousState.timeAtRead);
  state.consumedEnergyTotal += averagePower * timeElapsed;

  previousState = state;

  return state;
}

State Jetson_::measure() { return read_jetson(); }

}  // end namespace jetson
}  // end namespace pmt
