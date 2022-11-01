#include "../common/pmt-test.h"

#include "Rapl.h"

int main(int argc, char **argv) {
  auto sensor = pmt::rapl::Rapl::create();
  run(*sensor, argc, argv);
}