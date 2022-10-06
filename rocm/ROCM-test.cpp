#include "../common/pmt-test.h"

#include "ROCM.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::rocm::ROCM::create(0);
  run(sensor, argc, argv);
  delete sensor;
}
