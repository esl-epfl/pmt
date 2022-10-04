#include <dirent.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <string>

#include <iostream>
#include <sstream>
#include <vector>

#include "Xilinx.h"

namespace pmt {
namespace xilinx {

class Xilinx_ : public Xilinx {
 public:
  Xilinx_(int device_number);

 private:
  class XilinxState {
   public:
    operator State();
    double timeAtRead;
    double instantaneousPower = 0;
    double consumedEnergyDevice = 0;
  };

  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/xilinxpmt.out"; }

  virtual int getDumpInterval() {
    return 100;  // milliseconds
  }

  std::string filename;

  XilinxState previousState;
  XilinxState read_xilinx();
};

Xilinx_::XilinxState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyDevice * 1e-6;
  return state;
}

Xilinx *Xilinx::create(int device_number) { return new Xilinx_(device_number); }

Xilinx_::Xilinx_(int device_number) {
  char *c_str_filename = std::getenv("PMT_DEVICE");
  if (c_str_filename) {
    filename = c_str_filename;
  } else {
    fprintf(stderr, "No PMT_DEVICE specified.\n");
    exit(1);
  }

  previousState = read_xilinx();
  previousState.consumedEnergyDevice = 0;
}

float get_power(std::string &filename) {
  // Open power file
  // e.g.
  // /sys/devices/pci0000:a0/0000:a0:03.1/0000:a1:00.0/hwmon/hwmon2/power1_input
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (errno != 0) {
    std::cerr << "Could not open: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  // This file has one line with instantenous power consumption in uW
  std::string line;
  std::getline(file, line);
  try {
    return stoi(line);
  } catch (std::invalid_argument &e) {
    std::cerr << "Could not parse: " << line << std::endl;
    exit(EXIT_FAILURE);
  }
}

Xilinx_::XilinxState Xilinx_::read_xilinx() {
  XilinxState state;
  state.timeAtRead = get_wtime();
  state.instantaneousPower = get_power(filename);
  state.consumedEnergyDevice = previousState.consumedEnergyDevice;
  float averagePower =
      (state.instantaneousPower + previousState.instantaneousPower) / 2;
  float timeElapsed = (state.timeAtRead - previousState.timeAtRead);
  state.consumedEnergyDevice += averagePower * timeElapsed;
  previousState = state;
  return state;
}

State Xilinx_::measure() { return read_xilinx(); }

}  // end namespace xilinx
}  // end namespace pmt
