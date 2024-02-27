#ifndef PMT_POWERSENSOR2_H_
#define PMT_POWERSENSOR2_H_

#include <memory>
#include <string>

#include "common/pmt.h"

namespace pmt::powersensor2 {

class PowerSensor2 : public PMT {
 public:
  static std::unique_ptr<PowerSensor2> Create(
      const char *device = default_device().c_str());
  static std::string default_device() { return "/dev/ttyACM0"; }
};

}  // end namespace pmt::powersensor2

#endif  // PMT_POWERSENSOR2_H_
