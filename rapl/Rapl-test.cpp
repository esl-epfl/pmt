#include <memory>

#include "../common/pmt-test.h"

#include "Rapl.h"

int main(int argc, char **argv) {
  auto sensor = pmt::rapl::Rapl::Create();
  run(*sensor, argc, argv);
}