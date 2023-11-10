#ifndef PMT_RAPL_H_
#define PMT_RAPL_H_

#include <memory>

#include "pmt.h"

namespace pmt::rapl {
class Rapl : public PMT {
 public:
  static std::unique_ptr<Rapl> Create();
};
}  // end namespace pmt::rapl

#endif  // PMT_RAPL_H_
