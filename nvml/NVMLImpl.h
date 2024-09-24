#include <nvml.h>
#include <cuda.h>

#include "NVML.h"
#include "common/PMT.h"

#if (CUDA_VERSION <= 12000)
#define PMT_NVML_LEGACY_MODE
#endif

namespace nvml {
class Context;
class Device;
}  // end namespace nvml
namespace pmt::nvml {

struct NVMLMeasurement {
  std::string name;
  unsigned int value;
  unsigned int timestamp;
};

class NVMLState {
 public:
  operator State();
  double timestamp_;
  std::vector<NVMLMeasurement> measurements_;
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
  std::vector<NVMLMeasurement> GetMeasurements();

#if not defined(PMT_NVML_LEGACY_MODE)
  const unsigned int kFieldIdPowerInstant = NVML_FI_DEV_POWER_INSTANT;
  const unsigned int kFieldIdPowerAverage = NVML_FI_DEV_POWER_AVERAGE;
  unsigned int nr_scopes_;
#endif
  bool stopped_ = false;

  std::unique_ptr<::nvml::Context> context_;
  std::unique_ptr<::nvml::Device> device_;
};

}  // end namespace pmt::nvml