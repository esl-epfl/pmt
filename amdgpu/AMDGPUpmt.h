#ifndef AMDGPU_PMT_H
#define AMDGPU_PMT_H

#include "pmt.h"

namespace pmt {
    namespace amdgpu {
        class AMDGPUpmt : public pmt {
            public:
                static AMDGPUpmt* create(
                    int device_number = 0);
        };
    } // end namespace amdgpu
} // end namespace pmt

#endif
