#include "../common/pmt-test.h"

#include "Arduino.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::arduino::Arduino::create(
      pmt::arduino::Arduino::default_device().c_str(),
      pmt::arduino::POWERSENSOR_VERSION::V3);
  run(*sensor, argc, argv);
}