#ifndef ARDUINO_PMT_H_
#define ARDUINO_PMT_H_

#include "pmt.h"


namespace pmt {
namespace arduino {

enum POWERSENSOR_VERSION{
  V2=2,
  V3=3
};

class Arduino : public PMT {
public:
  static std::unique_ptr<Arduino> create(const char *device = default_device().c_str(), POWERSENSOR_VERSION version=V2);
  static std::string default_device() { return "/dev/ttyACM0"; }
};
} // end namespace arduino
} // end namespace pmt

#endif
