#include <memory>

#include "../common/pmt-test.h"

#include "Tegra.h"

int main(int argc, char** argv) {
  auto sensor = pmt::tegra::Tegra::Create();
  run(*sensor, argc, argv);
}
