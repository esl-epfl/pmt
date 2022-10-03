#include "../common/pmt-test.h"

#include "Arduinopmt.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::arduino::Arduinopmt::create();
  run(sensor, argc, argv);
  delete sensor;
}