#ifndef ARDUINO_PMT_H_
#define ARDUINO_PMT_H_

#include "pmt.h"

namespace pmt {
    namespace arduino {
        class Arduinopmt : public pmt {
            public:
                static Arduinopmt* create(
                    const char *device = default_device().c_str());

                static std::string default_device() {
                    return "/dev/ttyACM0";
                }
        };
    } // end namespace arduino
} // end namespace pmt

#endif
