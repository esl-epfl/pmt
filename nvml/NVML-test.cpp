#include "../common/pmt-test.h"

#include "NVML.h"

int main(int argc, char **argv) {
  auto sensor = pmt::nvml::NVML::create();
  run(*sensor, argc, argv);
}
