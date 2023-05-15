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

std::unique_ptr<Jetson> Jetson::create() { return std::make_unique<Jetson_>(); }

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
      state.measurements[1].first.compare("CPU") == 0 &&
      state.measurements[2].first.compare("SOC") == 0 &&
      state.measurements[3].first.compare("CV") == 0 &&
      state.measurements[4].first.compare("VDDRQ") == 0 &&
      state.measurements[5].first.compare("SYS5V") == 0) {
    // AGX Xavier
    for (auto &measurement : state.measurements) {
      state.instantaneousPowerTotal += measurement.second;
    }
  } else if (state.measurements.size() == 6 &&
             state.measurements[0].first.compare("VDD_GPU_SOC") == 0 &&
             state.measurements[1].first.compare("VDD_CPU_CV") == 0 &&
             state.measurements[2].first.compare("VIN_SYS_5V0") == 0 &&
             state.measurements[3].first.compare("NC") == 0 &&
             state.measurements[4].first.compare("VDDQ_VDD2_1V8AO") == 0 &&
             state.measurements[5].first.compare("NC") == 0) {
    // AGX Orin
    for (auto &measurement : state.measurements) {
      state.instantaneousPowerTotal += measurement.second;
    }
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
