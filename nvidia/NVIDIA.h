#ifndef PMT_NVIDIA_H_
#define PMT_NVIDIA_H_

#include <memory>

#include "common/PMT.h"

namespace pmt::nvidia {
class NVIDIA : public PMT {
 public:
  inline static std::string name = "nvidia";
  static std::unique_ptr<PMT> Create(int device_number = 0);
};
}  // end namespace pmt::nvidia

#endif  // PMT_NVIDIA_H_
