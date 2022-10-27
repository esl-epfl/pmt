#ifndef XILINX_PMT_H
#define XILINX_PMT_H

#include "pmt.h"

namespace pmt {
namespace xilinx {
class Xilinx : public PMT {
 public:
  static std::unique_ptr<Xilinx> create(int device_number = 0);
};
}  // end namespace xilinx
}  // end namespace pmt
#endif
