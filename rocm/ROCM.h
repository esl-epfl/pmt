#ifndef ROCM_PMT_H
#define ROCM_PMT_H

#include "common/pmt.h"

namespace pmt {
namespace rocm {
class ROCM : public PMT {
 public:
  static std::unique_ptr<ROCM> Create(int device_number = 0);
};
}  // end namespace rocm
}  // end namespace pmt

#endif
