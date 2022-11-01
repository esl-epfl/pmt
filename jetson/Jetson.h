#ifndef JETSON_PMT_H
#define JETSON_PMT_H

#include "pmt.h"

namespace pmt {
namespace jetson {
class Jetson : public PMT {
public:
  static std::unique_ptr<Jetson> create();
};
} // end namespace jetson
} // end namespace pmt

#endif
