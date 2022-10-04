#ifndef DUMMY_PMT_H_
#define DUMMY_PMT_H_

#include "pmt.h"

namespace pmt {
class Dummy : public pmt {
public:
  static Dummy *create();
};
} // end namespace pmt

#endif
