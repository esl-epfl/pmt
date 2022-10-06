#include <fstream>
#include <iostream>

#include <omp.h>
#include <unistd.h>

#include "Jetson.h"

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
    unsigned int instantaneousPowerGPU = 0;
    unsigned int instantaneousPowerCPU = 0;
    unsigned int instantaneousPowerSOC = 0;
    unsigned int instantaneousPowerCV = 0;
    unsigned int instantaneousPowerDDR = 0;
    unsigned int instantaneousPowerSYS5V = 0;
    unsigned int instantaneousPowerTotal = 0;
    unsigned int consumedEnergyTotal = 0;
  };

  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/Jetson.out"; }

  virtual int getDumpInterval() {
    return 50;  // milliseconds
  }

  JetsonState previousState;
  JetsonState read_jetson();
};

Jetson_::JetsonState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyTotal * 1e-3;

  state.misc.reserve(7);
  state.misc.push_back(instantaneousPowerGPU);
  state.misc.push_back(instantaneousPowerCPU);
  state.misc.push_back(instantaneousPowerSOC);
  state.misc.push_back(instantaneousPowerCV);
  state.misc.push_back(instantaneousPowerDDR);
  state.misc.push_back(instantaneousPowerSYS5V);
  state.misc.push_back(instantaneousPowerTotal);

  return state;
}

Jetson *Jetson::create() { return new Jetson_(); }

Jetson_::Jetson_() {
  previousState = read_jetson();
  previousState.consumedEnergyTotal = 0;
}

Jetson_::JetsonState Jetson_::read_jetson() {
  JetsonState state;
  state.timeAtRead = get_wtime();

  std::ifstream gpuPowerFile, cpuPowerFile, SOCPowerFile, CVPowerFile,
      DDRPowerFile, SYS5VPowerFile;
  gpuPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0040/iio:device0/in_power0_input");
  cpuPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0040/iio:device0/in_power1_input");
  SOCPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0040/iio:device0/in_power2_input");
  CVPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0041/iio:device1/in_power0_input");
  DDRPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0041/iio:device1/in_power1_input");
  SYS5VPowerFile.open(
      "/sys/bus/i2c/drivers/ina3221x/1-0041/iio:device1/in_power2_input");

  std::string powerLine;

  // GPU power
  gpuPowerFile >> powerLine;
  state.instantaneousPowerGPU = atoi(powerLine.c_str());
  powerLine.clear();

  // CPU power
  cpuPowerFile >> powerLine;
  state.instantaneousPowerCPU = atoi(powerLine.c_str());
  powerLine.clear();

  // SOC power
  SOCPowerFile >> powerLine;
  state.instantaneousPowerSOC = atoi(powerLine.c_str());
  powerLine.clear();

  // CV power
  CVPowerFile >> powerLine;
  state.instantaneousPowerCV = atoi(powerLine.c_str());
  powerLine.clear();

  // DDR power
  DDRPowerFile >> powerLine;
  state.instantaneousPowerDDR = atoi(powerLine.c_str());
  powerLine.clear();

  // SYS5V power
  SYS5VPowerFile >> powerLine;
  state.instantaneousPowerSYS5V = atoi(powerLine.c_str());
  powerLine.clear();

  gpuPowerFile.close();
  cpuPowerFile.close();
  SOCPowerFile.close();
  CVPowerFile.close();
  DDRPowerFile.close();
  SYS5VPowerFile.close();

  state.instantaneousPowerTotal = 0;
  state.instantaneousPowerTotal += state.instantaneousPowerGPU;
  state.instantaneousPowerTotal += state.instantaneousPowerCPU;
  state.instantaneousPowerTotal += state.instantaneousPowerSOC;
  state.instantaneousPowerTotal += state.instantaneousPowerCV;
  state.instantaneousPowerTotal += state.instantaneousPowerDDR;
  state.instantaneousPowerTotal += state.instantaneousPowerSYS5V;

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
