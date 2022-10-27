#ifndef LIKWID_PMT_H
#define LIKWID_PMT_H

#include "pmt.h"

namespace pmt {
namespace likwid {
class Likwid : public PMT {
public:
  static Likwid *create();
};
} // end namespace likwid
} // end namespace pmt

#endif
