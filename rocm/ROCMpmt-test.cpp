#include "../common/pmt-test.h"

#include "ROCMpmt.h"

int main(int argc, char *argv[])
{
    auto sensor = pmt::rocm::ROCMpmt::create(0);
    run(sensor, argc, argv);
    delete sensor;
}
