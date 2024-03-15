#ifndef TegraImplPMT_H
#define TegraImplPMT_H

#include <memory>

#include "common/pmt.h"

namespace pmt::tegra {
class Tegra : public PMT {
 public:
  inline static std::string name = "tegra";
  static std::unique_ptr<Tegra> Create();
};
}  // end namespace pmt::tegra

#endif
