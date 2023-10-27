#ifndef PMT_H_
#define PMT_H_

#include "pmt/pmt-config.h"

#include "pmt/pmt.h"
#if defined(HAVE_POWERSENSOR2)
#include "pmt/PowerSensor2.h"
#endif
#if defined(HAVE_POWERSENSOR3)
#include "pmt/PowerSensor3.h"
#endif
#include "pmt/Dummy.h"
#if defined(HAVE_NVML)
#include "pmt/NVML.h"
#endif
#if defined(HAVE_ROCMSMI)
#include "pmt/ROCM.h"
#endif
#include "pmt/Rapl.h"
#include "pmt/Tegra.h"
#include "pmt/Xilinx.h"

#endif
