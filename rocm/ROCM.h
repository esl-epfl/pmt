#ifndef ROCM_PMT_H
#define ROCM_PMT_H

#include "pmt.h"

namespace pmt {
namespace rocm {
class ROCM : public pmt {
 public:
  static ROCM *create(int device_number = 0);
};
}  // end namespace rocm
}  // end namespace pmt

#endif
