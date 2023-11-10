#include <memory>

#include "../common/pmt-test.h"

#include "Cray.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::cray::Cray::Create();
  run(*sensor, argc, argv);
}
