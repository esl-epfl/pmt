#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <ext/alloc_traits.h>

#include "NVMLImpl.h"

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

NVMLState::operator State() {
  State state(measurements_.size());
  state.timestamp_ = timestamp_;
  state.joules_[0] = joules_ * 1e-3;
  for (size_t i = 0; i < measurements_.size(); i++) {
    state.name_[i] = measurements_[i].name;
    state.watt_[i] = measurements_[i].value * 1e-3;
  }
  return state;
}

NVMLImpl::NVMLImpl(int device_number) {
  const char *pmt_device = getenv("PMT_DEVICE");
  device_number = pmt_device ? atoi(pmt_device) : device_number;

  checkNVMLCall(nvmlInit());
  checkNVMLCall(nvmlDeviceGetHandleByIndex(device_number, &device_));

  // Check whether the CPU+GPU scope is supported (e.g. Grace Hopper)
  nvmlFieldValue_t values[1];
  values[0].fieldId = kFieldIdPowerAverage;
  values[0].scopeId = 1;
  checkNVMLCall(nvmlDeviceGetFieldValues(device_, 1, values));
  nr_scopes_ = 1 + (values[0].nvmlReturn == NVML_SUCCESS);
}

NVMLImpl::~NVMLImpl() { checkNVMLCall(nvmlShutdown()); }

std::vector<NVMLMeasurement> NVMLImpl::GetMeasurements() {
  const int nr_field_ids = 2;
  const int nr_measurements = nr_scopes_ * nr_field_ids;
  nvmlFieldValue_t values[nr_measurements];
  const unsigned int field_ids[] = {kFieldIdPowerInstant, kFieldIdPowerAverage};

  std::vector<NVMLMeasurement> measurements(nr_measurements);

  for (int i = 0; i < nr_measurements; i += nr_field_ids) {
    const unsigned int scopeId = i / nr_field_ids;
    values[i].fieldId = field_ids[0];
    values[i].scopeId = scopeId;
    values[i + 1].fieldId = field_ids[1];
    values[i + 1].scopeId = scopeId;
  }

  checkNVMLCall(nvmlDeviceGetFieldValues(device_, nr_measurements, values));

  const std::string scopeNames[] = {"gpu", "module"};
  const std::string suffixes[] = {"_instant", "_average"};

  for (int i = 0; i < nr_scopes_; ++i) {
    for (int j = 0; j < nr_field_ids; ++j) {
      NVMLMeasurement m;
      m.name = scopeNames[i] + suffixes[j];
      m.value = values[nr_field_ids * i + j].value.uiVal;
      m.timestamp = values[nr_field_ids * i + j].timestamp;
      measurements[nr_field_ids * i + j] = m;
    }
  }

  return measurements;
}

NVMLState NVMLImpl::GetNVMLState() {
  NVMLState state;
  try {
    state.timestamp_ = GetTime();
    state.measurements_ = GetMeasurements();

    // Default: use use the instantaneous GPU power
    // Grace Hopper: use the instantaneous module power
    const unsigned int measurement_id = nr_scopes_ == 1 ? 0 : 2;
    state.watt_ = state.measurements_[measurement_id].value;
    state.timestamp_ = state.measurements_[measurement_id].timestamp / 1.0e6;

    // Set derived fields of state
    state.joules_ = state_previous_.joules_;
    const float watt = (state.watt_ + state_previous_.watt_) / 2;
    const float duration = (state.timestamp_ - state_previous_.timestamp_);
    state.joules_ += watt * duration;
    state.watt_ = watt;

    state_previous_ = state;
  } catch (std::runtime_error &e) {
    return state_previous_;
  }

  return state;
}
}  // end namespace pmt::nvml