#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <cuda_runtime.h>
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

#define checkCudaCall(val) __checkCudaCall((val), #val, __FILE__, __LINE__)

inline void __checkCudaCall(cudaError_t result, const char *const func,
                            const char *const file, int const line) {
  if (result != cudaSuccess) {
    std::stringstream error;
    error << "CUDA Error at " << file;
    error << ":" << line;
    error << " in function " << func;
    error << ": " << cudaGetErrorString(result);
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

/*
 * Convert a cudaUUID_t to CUDA's string representation.
 * The cudaUUID_t contains an array of 16 bytes, the UUID has
 * the form * 'GPU-XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXX', with every
 * X being * an alphanumeric character.
 */
std::string to_string(cudaUUID_t uuid) {
  std::stringstream result;
  result << "GPU";

  for (int i = 0; i < 16; ++i) {
    if (i == 0 || i == 4 || i == 6 || i == 8 || i == 10) {
      result << "-";
    }
    result << std::hex << std::setfill('0') << std::setw(2)
           << static_cast<unsigned>(static_cast<unsigned char>(uuid.bytes[i]));
  }

  return result.str();
}

NVMLImpl::NVMLImpl(int device_number) {
  const char *pmt_device = getenv("PMT_DEVICE");
  device_number = pmt_device ? atoi(pmt_device) : device_number;

  // Get the UUID of the CUDA device with the specified device number
  cudaDeviceProp prop;
  checkCudaCall(cudaGetDeviceProperties(&prop, device_number));

  // Get the string representation of the UUID
  const std::string uuid = to_string(prop.uuid);

  // Initialize NVML and get the device handle corresponding to the CUDA device
  checkNVMLCall(nvmlInit());
  checkNVMLCall(nvmlDeviceGetHandleByUUID(uuid.c_str(), &device_));

  // Check whether the CPU+GPU scope is supported (e.g. Grace Hopper)
  nvmlFieldValue_t values[1];
  values[0].fieldId = kFieldIdPowerAverage;
  values[0].scopeId = 1;
  checkNVMLCall(nvmlDeviceGetFieldValues(device_, 1, values));
  nr_scopes_ = 1 + (values[0].nvmlReturn == NVML_SUCCESS);
}

NVMLImpl::~NVMLImpl() {
  stopped_ = true;
  checkNVMLCall(nvmlShutdown());
}

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
      int idx = nr_field_ids * i + j;
      measurements[idx].name = scopeNames[i] + suffixes[j];
      measurements[idx].value = values[idx].value.uiVal;
      measurements[idx].timestamp = values[idx].timestamp;
    }
  }

  return measurements;
}

NVMLState NVMLImpl::GetNVMLState() {
  if (stopped_) {
    return state_previous_;
  }

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