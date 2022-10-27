#ifndef NVML_PMT_H_
#define NVML_PMT_H_

#include "pmt.h"

namespace pmt {
namespace nvml {
class NVML : public PMT {
public:
  static NVML *create(int device_number = 0);
};
} // end namespace nvml
} // end namespace pmt

#endif
