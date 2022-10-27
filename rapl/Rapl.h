#ifndef RAPL_PMT_H
#define RAPL_PMT_H

#include "pmt.h"

namespace pmt {
namespace rapl {
class Rapl : public PMT {
public:
  static std::unique_ptr<Rapl> create();
};
} // end namespace rapl
} // end namespace pmt

#endif
