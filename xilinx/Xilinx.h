#ifndef XILINX_PMT_H
#define XILINX_PMT_H

#include "pmt.h"

namespace pmt {
namespace xilinx {
class Xilinx : public pmt {
 public:
  static Xilinx *create(int device_number = 0);
};
}  // end namespace xilinx
}  // end namespace pmt
#endif
