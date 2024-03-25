#ifndef CRAY_PMT_H
#define CRAY_PMT_H

#include <cstddef>
#include <memory>
#include <string>

#include "common/PMT.h"

namespace pmt {
namespace cray {

struct CrayMeasurement {
  std::string name;
  size_t watt;
};

class Cray : public PMT {
 public:
  inline static std::string Cray::name = "cray";
  static std::unique_ptr<Cray> Create();
};
}  // end namespace cray
}  // end namespace pmt

#endif
