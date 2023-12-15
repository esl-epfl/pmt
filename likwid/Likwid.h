#ifndef LIKWID_PMT_H
#define LIKWID_PMT_H

#include <memory>
#include "pmt.h"

namespace pmt {
namespace likwid {
class Likwid : public PMT {
 public:
  static std::unique_ptr<Likwid> Create();
};
}  // end namespace likwid
}  // end namespace pmt

#endif
