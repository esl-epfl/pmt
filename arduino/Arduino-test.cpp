#include "../common/pmt-test.h"

#include "Arduino.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::arduino::Arduino::create();
  run(*sensor, argc, argv);
}