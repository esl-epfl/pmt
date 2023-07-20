#ifndef PMT_DUMMY_H_
#define PMT_DUMMY_H_

#include "pmt.h"

namespace pmt {
class Dummy : public PMT {
public:
  static std::unique_ptr<Dummy> Create();
};
} // end namespace pmt

#endif // PMT_DUMMY_H_
