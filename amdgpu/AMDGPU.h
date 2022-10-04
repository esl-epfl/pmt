#ifndef AMDGPU_PMT_H
#define AMDGPU_PMT_H

#include "pmt.h"

namespace pmt {
namespace amdgpu {
class AMDGPU : public pmt {
public:
  static AMDGPU *create(int device_number = 0);
};
} // end namespace amdgpu
} // end namespace pmt

#endif
