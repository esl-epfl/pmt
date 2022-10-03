#ifndef DUMMY_PMT_H_
#define DUMMY_PMT_H_

#include "pmt.h"

namespace pmt {
class Dummypmt : public pmt {
public:
  static Dummypmt *create();
};
} // end namespace pmt

#endif
