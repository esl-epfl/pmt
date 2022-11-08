# PMT: Power Measurement Toolkit
PMT is a high-level software library capable of collecting power consumption measurements on various hardware. The library provides a standard interface to easily measure the energy use of devices such as CPUs and GPUs in critical application sections.

# Installation
First clone the repository:
```
git clone --recursive git@git.astron.nl:RD/pmt.git
```
The `--recursive` flag makes sure that the git submodules are also cloned. If you cloned the repository without this flag, you can initialize the submodules as follows:
```
git submodule update --init
```
To build the software, run the following commands:
1. Set-up cmake in a build directory and cd into the directory, e.g. `~/pmt/build`
     * `cmake <source dir path> -DCMAKE_INSTALL_PREFIX=<install dir path>`
2. Optionally further configure cmake through interactive build settings or with command line variables
     * use `ccmake` and/or add `-DBUILD_<SENSOR>_PMT=<0 or 1>` to the `cmake` commandline to select which PMT are built.
3. make and install
     * `make install`

# Usage
Include the header file into your program, e.g.:
```
#include “pmt.h”
```
Depending on which PMT implementations you have selected during the build, you can now initialize
any PMT instance:
```
std::unique ptr<pmt::PMT> sensor(pmt::nvml::NVML::create());
```
Next, you can start measuring power using the common api as specified in `pmt.h`:
```
pmt::State start, end;
start = sensor−>read();
...
end = sensor−>read () ;
std::cout<<sensor−>joules(start, end) <<” [J]“<<std::endl;
std::cout<<sensor−>watts(start, end) <<” [W]“<<std::endl;
std::cout<<sensor−>seconds(start, end)<<” [S]“<<std::endl;
```

# pmt-test
PMT-test executables might be used direclty to read the values of a PowerSensor at a regular interval. The PMT-test executables are available in the install directory `/bin`.

# Acknowledgement
If you decide to use PMT in your research, please cite the following reference:

```
@misc{https://doi.org/10.48550/arxiv.2210.03724,
  doi = {10.48550/ARXIV.2210.03724},
  
  url = {https://arxiv.org/abs/2210.03724},
  
  author = {Corda, Stefano and Veenboer, Bram and Tolley, Emma},
  
  keywords = {Performance (cs.PF), FOS: Computer and information sciences, FOS: Computer and information sciences},
  
  title = {PMT: Power Measurement Toolkit},
  
  publisher = {arXiv},
  
  year = {2022},
  
  copyright = {arXiv.org perpetual, non-exclusive license}
}
```
