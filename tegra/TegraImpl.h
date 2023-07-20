#include <string>
#include <vector>

#include "Tegra.h"

namespace pmt::tegra {

using TegraMeasurement = std::pair<std::string, int>;

class TegraState {
 public:
  operator State();
  double timestamp_;
  std::vector<std::pair<std::string, int>> measurements;
  unsigned int watt_ = 0;
  unsigned int joules_ = 0;
};

class TegraImpl : public Tegra {
 public:
  TegraImpl();
  ~TegraImpl();

   State GetState() override { return GetTegraState(); }

  virtual const char *GetDumpFilename() { return "/tmp/pmt_tegra.out"; }
  virtual int GetMeasurementInterval() { return measurement_interval_; }

private:
  TegraState GetTegraState();
  std::vector<TegraMeasurement> GetMeasurements();

  std::string filename_ = "";
  bool started_tegrastats_ = false;
  const int measurement_interval_ = 10;  // milliseconds
  TegraState previous_state_;
};

}  // end namespace pmt::tegra