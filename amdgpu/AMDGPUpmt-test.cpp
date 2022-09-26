#include "../common/pmt-test.h"

#include "AMDGPUpmt.h"

int main(int argc, char *argv[])
{
    auto sensor = pmt::amdgpu::AMDGPUpmt::create();
    run(sensor, argc, argv);
    delete sensor;
}
