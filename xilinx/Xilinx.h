#ifndef PMT_XILINX_H_
#define PMT_XILINX_H_

#include "pmt.h"

#include <memory>
#include <string>

namespace pmt::xilinx {
class Xilinx : public PMT {
 public:
  static std::unique_ptr<Xilinx> Create(
      const char *device = default_device().c_str());
  static std::string default_device() {
    return "/sys/devices/pci0000:a0/0000:a0:03.1/0000:a1:00.0/hwmon/hwmon3/"
           "power1_input";
  }
};
}  // end namespace pmt::xilinx

#endif  // PMT_XILINX_H_
