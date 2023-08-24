#ifndef CRAY_PMT_H
#define CRAY_PMT_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "FilenamesHelper.h"
#include "pmt.h"

namespace pmt {
namespace cray {

struct CrayMeasurement {
  std::string name;
  size_t watt;
};

class Cray : public PMT {
 public:
  static std::unique_ptr<Cray> Create();
};
}  // end namespace cray
}  // end namespace pmt

#endif
