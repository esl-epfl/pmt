#include <string>
#include <vector>

#include "Tegra.h"

namespace pmt::tegra {

using TegraMeasurement = std::pair<std::string, int>;

class TegraState {
 public:
  operator State();
  double timeAtRead;
  std::vector<std::pair<std::string, int>> measurements;
  unsigned int instantaneousPowerTotal = 0;
  unsigned int consumedEnergyTotal = 0;
};

class TegraImpl : public Tegra {
 public:
  TegraImpl();
  ~TegraImpl();

   State read() override { return GetTegraState(); }

  virtual const char *getDumpFileName() { return "/tmp/pmt_tegra.out"; }
  virtual int getMeasurementInterval() { return measurement_interval_; }

private:
  TegraState GetTegraState();
  std::vector<TegraMeasurement> GetMeasurements();

  std::string filename_ = "";
  bool started_tegrastats_ = false;
  const int measurement_interval_ = 10;  // milliseconds
  TegraState previous_state_;
};

}  // end namespace pmt::tegra