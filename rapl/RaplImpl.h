#ifndef PMT_RAPLIMPL_H_
#define PMT_RAPLIMPL_H_

#include <cassert>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Rapl.h"

namespace pmt::rapl {

const int kNumRaplDomains = 4;
const int kKeepAliveInterval = 10; // call Measure() roughly every nth update

struct RaplMeasurement {
  std::string name;
  size_t value;
};

  class RaplState {
   public:
    operator State();
    double timeAtRead = 0;
    std::vector<RaplMeasurement> measurements;

    size_t consumedEnergyTotal;
  };

class RaplImpl : public Rapl {
public:
  RaplImpl();
  ~RaplImpl();

  State read() override { return GetRaplState(); }

  virtual const char *getDumpFileName() { return "/tmp/pmt_rapl.out"; }
  virtual int getMeasurementInterval() { return measurement_interval_; }

private:
  std::vector<int> DetectPackages();
  void Init();
  void SetMeasurementInterval();
  void StartThread();
  void StopThread();
  RaplState GetRaplState();
  std::vector<RaplMeasurement> GetMeasurements();

  std::vector<std::string> domain_names_;
  std::vector<std::string> file_names_;
  int measurement_interval_;

  // The numbers in the rapl /energy_uj files range from zero up to a maximum
  // specified in /max_energy_range_uj. This class reports monotonically
  // increasing values starting with zero for the first measurement. Therefore,
  // for every counter, the result is computed as follows:
  //  now = <read value>
  //  total += now < previous ? max : 0
  //  result += total + now - first
  std::vector<size_t> uj_max_;
  std::vector<size_t> uj_first_;
  std::vector<size_t> uj_previous_;
  std::vector<size_t> uj_total_;

  // Mutex used to guard Measure()
  std::mutex mutex_;

  // To prevent /energy_uj from overflowing without RaplImpl noticing, a thread
  // periodically calls Measure() to keep the internal state up-to-date.
  std::thread thread_;
  volatile bool stop_;
};

} // end namespace pmt::rapl

#endif // PMT_RAPLIMPL_H_
