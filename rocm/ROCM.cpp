#include <rocm_smi/rocm_smi.h>

#include "ROCM.h"

namespace {

size_t GetPower(uint32_t device_number) {
  rsmi_status_t status;
  uint64_t power;  // in mW
  const uint32_t sensor_number = 0;
  status = rsmi_dev_power_ave_get(device_number, sensor_number, &power);

  if (status == RSMI_STATUS_PERMISSION || status != RSMI_STATUS_SUCCESS) {
    throw std::runtime_error("ROCM-SMI read failed");
  }

  return static_cast<double>(power) * 1e-6;  // in W
}
}  // namespace

namespace pmt::rocm {

class ROCMState {
 public:
  operator State();
  double timestamp_;
  double watt_ = 0;
  double joules_ = 0;
};

class ROCMImpl : public ROCM {
 public:
  ROCMImpl(const unsigned device_number);
  ~ROCMImpl();

 private:
  virtual State GetState();

  virtual const char *GetDumpFilename() { return "/tmp/pmt_rocm.out"; }

  virtual int GetMeasurementInterval() {
    return 100;  // milliseconds
  }

  unsigned int device_number_;

  ROCMState state_previous_;
  ROCMState GetROCMState();
};

std::unique_ptr<ROCM> ROCM::Create(int device_number) {
  return std::make_unique<ROCMImpl>(device_number);
}

ROCMImpl::ROCMImpl(const unsigned device_number) {
  rsmi_status_t status;
  status = rsmi_init(0);
  if (status == RSMI_STATUS_PERMISSION || status != RSMI_STATUS_SUCCESS) {
    throw std::runtime_error("ROCM-SMI initialization failed");
  }

  device_number_ = device_number;

  state_previous_ = GetROCMState();
  state_previous_.joules_ = 0;
}

ROCMImpl::~ROCMImpl() { rsmi_shut_down(); }

ROCMState::operator State() {
  State state;
  state.timestamp_ = timestamp_;
  state.name_[0] = "device";
  state.joules_[0] = joules_;
  state.watt_[0] = watt_;
  return state;
}

ROCMState ROCMImpl::GetROCMState() {
  ROCMState state;
  state.timestamp_ = GetTime();
  state.watt_ = GetPower(device_number_);
  state.joules_ = state_previous_.joules_;
  const float watt = (state.watt_ + state_previous_.watt_) / 2;
  const float duration = (state.timestamp_ - state_previous_.timestamp_);
  state.joules_ += watt * duration;
  state_previous_ = state;
  return state;
}

State ROCMImpl::GetState() { return GetROCMState(); }

}  // end namespace pmt::rocm
