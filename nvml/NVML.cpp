#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <ext/alloc_traits.h>

#include "NVML.h"

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

namespace pmt::nvml {
class NVMLState {
 public:
  operator State();
  double timestamp_;
  unsigned int watt_ = 0;
  unsigned int joules_ = 0;
};

class NVMLImpl : public NVML {
 public:
  NVMLImpl(int device_number);
  ~NVMLImpl();

 private:
  State GetState() override { return GetNVMLState(); }

  virtual const char *GetDumpFilename() override { return "/tmp/pmt_nvml.out"; }

  virtual int GetMeasurementInterval() override {
    return 10;  // milliseconds
  }

  NVMLState state_previous_;
  NVMLState GetNVMLState();

  nvmlDevice_t device_;
};

NVMLState::operator State() {
  State state;
  state.timestamp_ = timestamp_;
  state.name_[0] = "device";
  state.joules_[0] = joules_ * 1e-3;
  state.watt_[0] = watt_ * 1e-3;
  return state;
}

std::unique_ptr<NVML> NVML::Create(int device_number) {
  return std::unique_ptr<NVML>(new NVMLImpl(device_number));
}

NVMLImpl::NVMLImpl(int device_number) {
  const char *pmt_device = getenv("PMT_DEVICE");
  device_number = pmt_device ? atoi(pmt_device) : device_number;

  checkNVMLCall(nvmlInit());
  checkNVMLCall(nvmlDeviceGetHandleByIndex(device_number, &device_));

  state_previous_ = GetNVMLState();
  state_previous_.joules_ = 0;
}

NVMLImpl::~NVMLImpl() { checkNVMLCall(nvmlShutdown()); }

NVMLState NVMLImpl::GetNVMLState() {
  NVMLState state;
  state.timestamp_ = GetTime();

  checkNVMLCall(nvmlDeviceGetPowerUsage(device_, &state.watt_));
  state.joules_ = state_previous_.joules_;
  const float watt = (state.watt_ + state_previous_.watt_) / 2;
  const float duration = (state.timestamp_ - state_previous_.timestamp_);
  state.joules_ += watt * duration;
  state.watt_ = watt;

  state_previous_ = state;

  return state;
}
}  // end namespace pmt::nvml
