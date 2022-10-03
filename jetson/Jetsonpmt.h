#ifndef JETSON_PMT_H
#define JETSON_PMT_H

#include "pmt.h"

namespace pmt {
namespace jetson {
class Jetsonpmt : public pmt {
public:
  static Jetsonpmt *create();
};
} // end namespace jetson
} // end namespace pmt

#endif
