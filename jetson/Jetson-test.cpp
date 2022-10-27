#include "../common/pmt-test.h"

#include "Jetson.h"

int main(int argc, char** argv) {
  auto sensor = pmt::jetson::Jetson::create();
  run(*sensor, argc, argv);
}
