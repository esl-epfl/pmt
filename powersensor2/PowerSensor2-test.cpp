#include "../common/pmt-test.h"

#include "PowerSensor2.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::powersensor2::PowerSensor2::create();
  run(*sensor, argc, argv);
}
