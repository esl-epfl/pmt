#include "../common/pmt-test.h"

#include "Likwidpmt.h"

int main(int argc, char **argv) {
  auto sensor = pmt::likwid::Likwidpmt::create();
  run(sensor, argc, argv);
  delete sensor;
}
