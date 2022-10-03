#include "../common/pmt-test.h"

#include "NVMLpmt.h"

int main(int argc, char **argv) {
  auto sensor = pmt::nvml::NVMLpmt::create();
  run(sensor, argc, argv);
  delete sensor;
}
