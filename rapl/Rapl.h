#ifndef RAPL_PMT_H
#define RAPL_PMT_H

#include "pmt.h"

namespace pmt {
namespace rapl {
class Rapl : public pmt {
public:
  static Rapl *create();
};
} // end namespace rapl
} // end namespace pmt

#endif
