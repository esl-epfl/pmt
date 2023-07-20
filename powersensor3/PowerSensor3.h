#ifndef PMT_POWERSENSOR3_H_
#define PMT_POWERSENSOR3_H_

#include "pmt.h"

namespace pmt::powersensor3 {

class PowerSensor3 : public PMT {
 public:
  static std::unique_ptr<PowerSensor3> Create(
      const char *device = default_device().c_str());
  static std::string default_device() { return "/dev/ttyACM0"; }
};

}  // end namespace pmt::powersensor3

#endif // PMT_POWERSENSOR3_H_
