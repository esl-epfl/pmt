#include "../common/pmt-test.h"

#include "Jetsonpmt.h"

int main(int argc, char **argv) {
  auto sensor = pmt::jetson::Jetsonpmt::create();
  run(sensor, argc, argv);
  delete sensor;
}
