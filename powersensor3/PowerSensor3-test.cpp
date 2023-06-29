#include "../common/pmt-test.h"

#include "PowerSensor3.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::powersensor3::PowerSensor3::create();
  run(*sensor, argc, argv);
}
