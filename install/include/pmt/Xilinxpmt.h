#ifndef XILINX_PMT_H
#define XILINX_PMT_H

#include "pmt.h"

namespace pmt {
    namespace xilinx {
        class Xilinxpmt : public pmt {
            public:
                static Xilinxpmt* create(
                    int device_number = 0);
        };
    } // end namespace xilinx
} // end namespace pmt
#endif
