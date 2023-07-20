#include "pmt-config.h"

#include "pmt/pmt.h"
#if defined(HAVE_POWERSENSOR2)
#include "pmt/PowerSensor2.h"
#endif
#if defined(HAVE_POWERSENSOR3)
#include "pmt/PowerSensor3.h"
#endif
#include "pmt/Dummy.h"
#include "pmt/Jetson.h"
#if defined(HAVE_NVML)
#include "pmt/NVML.h"
#endif
#if defined(HAVE_ROCMSMI)
#include "pmt/ROCM.h"
#endif
#include "pmt/Rapl.h"
#include "pmt/Xilinx.h"
