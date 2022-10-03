#ifndef LIKWID_PMT_H
#define LIKWID_PMT_H

#include "pmt.h"

namespace pmt {
namespace likwid {
class Likwidpmt : public pmt {
public:
  static Likwidpmt *create();
};
} // end namespace likwid
} // end namespace pmt

#endif
