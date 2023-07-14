#ifndef TegraImplPMT_H
#define TegraImplPMT_H

#include "pmt.h"

namespace pmt::tegra {
class Tegra : public PMT {
public:
  static std::unique_ptr<Tegra> create();
  std::vector<std::pair<std::string, double>> misc(const State &firstState, const State &secondState) override;
};
} // end namespace pmt::tegra

#endif
