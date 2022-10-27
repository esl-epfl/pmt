#ifndef DUMMY_PMT_H_
#define DUMMY_PMT_H_

#include "pmt.h"

namespace pmt {
class Dummy : public PMT {
public:
  static Dummy *create();
};
} // end namespace pmt

#endif
