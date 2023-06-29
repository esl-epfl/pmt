#ifndef POWERSENSOR2_PMT_H_
#define POWERSENSOR2_PMT_H_

#include "pmt.h"

namespace pmt {
namespace powersensor2 {

class PowerSensor2 : public PMT {
 public:
  static std::unique_ptr<PowerSensor2> create(
      const char *device = default_device().c_str());
  static std::string default_device() { return "/dev/ttyACM0"; }
};
}  // end namespace powersensor2
}  // end namespace pmt

#endif // POWERSENSOR2_PMT_H_
