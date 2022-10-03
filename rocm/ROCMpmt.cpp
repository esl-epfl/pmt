#include <dirent.h>
#include <string.h>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "rocm_smi/rocm_smi.h"

#include "ROCMpmt.h"

namespace pmt {
namespace rocm {

class ROCMpmt_ : public ROCMpmt {
public:
  ROCMpmt_(const unsigned device_number);
  ~ROCMpmt_();

private:
  class ROCMState {
  public:
    operator State();
    double timeAtRead;
    double instantaneousPower = 0;
    double consumedEnergyDevice = 0;
  };

  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/rocmpmt.out"; }

  virtual int getDumpInterval() {
    return 10; // milliseconds
  }

  unsigned int _device_number;

  ROCMState previousState;
  ROCMState read_rocm();
};

ROCMpmt_::ROCMState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyDevice;
  return state;
}

ROCMpmt *ROCMpmt::create(int device_number) {
  rsmi_status_t ret;
  ret = rsmi_init(0);
  if (ret == RSMI_STATUS_PERMISSION || ret != RSMI_STATUS_SUCCESS) {
    std::cout << "ROCM-SMI initialization failed" << std::endl;
    exit(EXIT_FAILURE);
  }
  return new ROCMpmt_(device_number);
}

ROCMpmt_::ROCMpmt_(const unsigned device_number) {
  _device_number = device_number;

  State startState = read_rocm();
}

float get_power(unsigned device_number) {
  rsmi_status_t ret;
  uint64_t val_ui64;
  uint32_t i = 0;
  ret = rsmi_dev_power_ave_get(device_number, 0, &val_ui64);

  if (ret == RSMI_STATUS_PERMISSION || ret != RSMI_STATUS_SUCCESS) {
    std::cout << "ROCM-SMI read failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  return static_cast<float>(val_ui64) * 1e-6;
}

ROCMpmt_::ROCMState ROCMpmt_::read_rocm() {
  ROCMState state;
  state.timeAtRead = get_wtime();
  state.instantaneousPower = get_power(_device_number);
  state.consumedEnergyDevice = previousState.consumedEnergyDevice;
  float averagePower =
      (state.instantaneousPower + previousState.instantaneousPower) / 2;
  float timeElapsed = (state.timeAtRead - previousState.timeAtRead);
  state.consumedEnergyDevice += averagePower * timeElapsed;
  previousState = state;
  return state;
}

ROCMpmt_::~ROCMpmt_() {
  stopDumpThread();
  rsmi_shut_down();
}

State ROCMpmt_::measure() { return read_rocm(); }

} // end namespace rocm
} // end namespace pmt
