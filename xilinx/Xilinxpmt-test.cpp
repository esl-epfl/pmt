#include "../common/pmt-test.h"

#include "Xilinxpmt.h"

int main(int argc, char **argv) {
  auto sensor = pmt::xilinx::Xilinxpmt::create();
  run(sensor, argc, argv);
  delete sensor;
}
