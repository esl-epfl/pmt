#include <memory>

#include "../common/pmt-test.h"

#include "NVIDIA.h"

int main(int argc, char **argv) {
  auto sensor = pmt::nvidia::NVIDIA::Create();
  run(*sensor, argc, argv);
}
