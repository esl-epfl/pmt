#ifndef RAPL_PMT_H
#define RAPL_PMT_H

#include "pmt.h"

namespace pmt {
    namespace rapl {
        class Raplpmt : public pmt {
            public:
                static Raplpmt* create();
        };
    } // end namespace rapl
} // end namespace pmt

#endif
