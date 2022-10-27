#include "../common/pmt-test.h"

#include "Xilinx.h"

int main(int argc, char **argv) {
  auto sensor = pmt::xilinx::Xilinx::create();
  run(*sensor, argc, argv);
}
