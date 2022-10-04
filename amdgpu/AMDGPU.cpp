#include <dirent.h>
#include <string.h>

#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

#include "AMDGPU.h"

namespace pmt {
namespace amdgpu {

class AMDGPU_ : public AMDGPU {
 public:
  AMDGPU_(const unsigned device_number);

 private:
  class AMDGPUState {
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

  AMDGPUState previousState;
  AMDGPUState read_amdgpu();
};

AMDGPU_::AMDGPUState::operator State() {
  State state;
  state.timeAtRead = timeAtRead;
  state.joulesAtRead = consumedEnergyDevice;
  return state;
}

AMDGPU *AMDGPU::create(int device_number) { return new AMDGPU_(device_number); }

AMDGPU_::AMDGPU_(const unsigned device_number) {
  // Power consumption is read from sysfs
  const char *dri_dir = "/sys/kernel/debug/dri";

  // Vector for all files (all GPUs)
  std::vector<std::string> pmfiles;

  // Try to find all AMD GPUs
  std::stringstream basedir;
  basedir << dri_dir << "/";
  DIR *d = opendir(basedir.str().c_str());
  struct dirent *dir;
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      std::stringstream pmfile;
      pmfile << basedir.str() << dir->d_name << "/amdgpu_pm_info";

      if (fopen(pmfile.str().c_str(), "r")) {
        pmfiles.push_back(pmfile.str());
      }
    }
    closedir(d);
  } else {
    fprintf(stderr, "Could not open directory: %s\n", dri_dir);
    exit(1);
  }

  // Select the file to read power consumption from
  filename = pmfiles[device_number];

  State startState = read_amdgpu();
}

float get_power(std::string &filename) {
  FILE *file = fopen(filename.c_str(), "r");
  char line[80];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strstr(line, "average GPU") != NULL) {
      const char delim[2] = " ";
      const char *token = strtok(line, delim);
      fclose(file);
      return atof(token);
    }
  }
  fclose(file);
  return -1;
}

AMDGPU_::AMDGPUState AMDGPU_::read_amdgpu() {
  AMDGPUState state;
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

State AMDGPU_::measure() { return read_amdgpu(); }

}  // end namespace amdgpu
}  // end namespace pmt
