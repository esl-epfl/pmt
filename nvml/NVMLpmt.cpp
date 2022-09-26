#include "NVMLpmt.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

#include <unistd.h>

#if defined(HAVE_NVML)
#include "nvml.h"

#define checkNVMLCall(val)  __checkNVMLCall((val), #val, __FILE__, __LINE__)

inline void __checkNVMLCall(
    nvmlReturn_t result,
    const char *const func,
    const char *const file,
    int const line)
{
    if (result != NVML_SUCCESS) {
        std::stringstream error;
        error << "NVML Error at " << file;
        error << ":" << line;
        error << " in function " << func;
        error << ": " << nvmlErrorString(result);
        error << std::endl;
        throw std::runtime_error(error.str());
    }
}
#endif

namespace pmt {
namespace nvml {

class NVMLpmt_ : public NVMLpmt {
    public:
        NVMLpmt_(int device_number);
        ~NVMLpmt_();

    private:
        class NVMLState {
            public:
                operator State();
                double timeAtRead;
                unsigned int instantaneousPower   = 0;
                unsigned int consumedEnergyDevice = 0;
        };

        virtual State measure();

        virtual const char* getDumpFileName() {
            return "/tmp/nvmlpmt.out";
        }

        virtual int getDumpInterval() {
            return 10; // milliseconds
        }

        NVMLState previousState;
        NVMLState read_nvml();

#if defined(HAVE_NVML)
        nvmlDevice_t device;
#endif
};

NVMLpmt_::NVMLState::operator State()
{
    State state;
    state.timeAtRead = timeAtRead;
    state.joulesAtRead = consumedEnergyDevice * 1e-3;
    return state;
}

NVMLpmt* NVMLpmt::create(
    int device_number)
{
    return new NVMLpmt_(device_number);
}

NVMLpmt_::NVMLpmt_(
    int device_number)
{
    char *cstr_pmt_device = getenv("PMT_DEVICE");
    unsigned device_number_ = cstr_pmt_device ? atoi(cstr_pmt_device) : device_number;

#if defined(HAVE_NVML)
    checkNVMLCall(nvmlInit());
    checkNVMLCall(nvmlDeviceGetHandleByIndex(device_number_, &device));
#endif

    previousState = read_nvml();
    previousState.consumedEnergyDevice = 0;
}

NVMLpmt_::~NVMLpmt_() {
#if defined(HAVE_NVML)
    stopDumpThread();
    checkNVMLCall(nvmlShutdown());
#endif
}

NVMLpmt_::NVMLState NVMLpmt_::read_nvml() {
    NVMLState state;
    state.timeAtRead = get_wtime();

#if defined(HAVE_NVML)
    checkNVMLCall(nvmlDeviceGetPowerUsage(device, &state.instantaneousPower));
    state.consumedEnergyDevice = previousState.consumedEnergyDevice;
    float averagePower = (state.instantaneousPower + previousState.instantaneousPower) / 2;
    float timeElapsed = (state.timeAtRead - previousState.timeAtRead);
    state.consumedEnergyDevice += averagePower * timeElapsed;
#else
    state.consumedEnergyDevice = 0;
#endif
    
    previousState = state;

    return state;
}

State NVMLpmt_::measure() {
    return read_nvml();
}

} // end namespace nvml
} // end namespace pmt
