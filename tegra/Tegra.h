#ifndef TegraImplPMT_H
#define TegraImplPMT_H

#include <memory>

#include "pmt.h"

namespace pmt::tegra {
class Tegra : public PMT {
 public:
  static std::unique_ptr<Tegra> Create();
};
}  // end namespace pmt::tegra

#endif
