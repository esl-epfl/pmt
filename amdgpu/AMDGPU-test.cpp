#include "../common/pmt-test.h"

#include "AMDGPU.h"

int main(int argc, char *argv[]) {
  auto sensor = pmt::amdgpu::AMDGPU::create();
  run(*sensor, argc, argv);
}
