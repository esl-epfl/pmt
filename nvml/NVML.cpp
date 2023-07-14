#include "NVML.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <unistd.h>

#if defined(HAVE_NVML)
#include "nvml.h"

#define checkNVMLCall(val) __checkNVMLCall((val), #val, __FILE__, __LINE__)

inline void __checkNVMLCall(nvmlReturn_t result, const char *const func,
                            const char *const file, int const line) {
  if (result != NVML_SUCCESS) {
    std::stringstream error;
    error << "NVML Error at " << file;
    error << ":" << line;
    error << " in function " << func;
    error << ": " << nvmlErrorString(result);
    error << std::endl;
    throw std::runtime_error(error.str());
  }
}
#endif

namespace pmt::nvml {

class NVMLState {
 public:
  operator State();
  double timeAtRead;
  unsigned int instantaneousPower = 0;
  unsigned int consumedEnergyDevice = 0;
};

class NVMLImpl : public NVML {
 public:
  NVMLImpl(int device_number);
  ~NVMLImpl();

 private:
  State read() override { return GetNVMLState(); }

  virtual const char *getDumpFileName() { return "/tmp/pmt_nvml.out"; }

  virtual int getMeasurementInterval() {
    return 10;  // milliseconds
  }

  NVMLState previousState;
  NVMLState GetNVMLState();

#if defined(HAVE_NVML)
  nvmlDevice_t device;
#endif
};

NVMLState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyDevice * 1e-3;
  return state;
}

std::unique_ptr<NVML> NVML::create(int device_number) {
  return std::unique_ptr<NVML>(new NVMLImpl(device_number));
}

NVMLImpl::NVMLImpl(int device_number) {
  const char *cstr_pmt_device = getenv("PMT_DEVICE");
  const unsigned device_number_ =
      cstr_pmt_device ? atoi(cstr_pmt_device) : device_number;

#if defined(HAVE_NVML)
  checkNVMLCall(nvmlInit());
  checkNVMLCall(nvmlDeviceGetHandleByIndex(device_number_, &device));
#endif

  previousState = GetNVMLState();
  previousState.consumedEnergyDevice = 0;
}

NVMLImpl::~NVMLImpl() {
#if defined(HAVE_NVML)
  stopDumpThread();
  checkNVMLCall(nvmlShutdown());
#endif
}

NVMLState NVMLImpl::GetNVMLState() {
  NVMLState state;
  state.timeAtRead = get_wtime();

#if defined(HAVE_NVML)
  checkNVMLCall(nvmlDeviceGetPowerUsage(device, &state.instantaneousPower));
  state.consumedEnergyDevice = previousState.consumedEnergyDevice;
  const float averagePower =
      (state.instantaneousPower + previousState.instantaneousPower) / 2;
  const float timeElapsed = (state.timeAtRead - previousState.timeAtRead);
  state.consumedEnergyDevice += averagePower * timeElapsed;
#else
  state.consumedEnergyDevice = 0;
#endif

  previousState = state;

  return state;
}

}  // end namespace pmt::nvml
