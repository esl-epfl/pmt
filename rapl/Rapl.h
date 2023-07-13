#ifndef PMT_RAPL_H_
#define PMT_RAPL_H_

#include "pmt.h"

namespace pmt::rapl {
class Rapl : public PMT {
public:
  static std::unique_ptr<Rapl> create();
  std::vector<std::pair<std::string, double>> misc(const State &firstState, const State &secondState) override;
};
} // end namespace pmt::rapl

#endif // PMT_RAPL_H_
