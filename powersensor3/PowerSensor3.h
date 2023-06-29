#ifndef POWERSENSOR3_PMT_H_
#define POWERSENSOR3_PMT_H_

#include "pmt.h"

namespace pmt {
namespace powersensor3 {

class PowerSensor3 : public PMT {
 public:
  static std::unique_ptr<PowerSensor3> create(
      const char *device = default_device().c_str());
  static std::string default_device() { return "/dev/ttyACM0"; }
};
}  // end namespace powersensor3
}  // end namespace pmt

#endif // POWERSENSOR3_PMT_H_
