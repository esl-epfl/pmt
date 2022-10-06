#include "../common/pmt-test.h"

#include "Likwid.h"

int main(int argc, char **argv) {
  auto sensor = pmt::likwid::Likwid::create();
  run(sensor, argc, argv);
  delete sensor;
}
