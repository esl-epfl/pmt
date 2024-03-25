#ifndef PMT_NVML_H_
#define PMT_NVML_H_

#include <memory>

#include "common/PMT.h"

namespace pmt::nvml {
class NVML : public PMT {
 public:
  inline static std::string name = "nvml";
  static std::unique_ptr<NVML> Create(int device_number = 0);
};
}  // end namespace pmt::nvml

#endif  // PMT_NVML_H_
