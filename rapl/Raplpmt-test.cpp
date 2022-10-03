#include "../common/pmt-test.h"

#include "Raplpmt.h"

int main(int argc, char **argv) {
  auto sensor = pmt::rapl::Raplpmt::create();
  run(sensor, argc, argv);
  delete sensor;
}