#ifndef JETSON_PMT_H
#define JETSON_PMT_H

#include "pmt.h"

namespace pmt {
namespace jetson {
class Jetson : public pmt {
public:
  static Jetson *create();
};
} // end namespace jetson
} // end namespace pmt

#endif
