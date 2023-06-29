#include "PowerSensor2.h"

#include POWERSENSOR2_HEADER

namespace {
double Seconds(const PowerSensor::State &first,
               const PowerSensor::State &second) {
  return PowerSensor::seconds(first, second);
}

double Joules(const PowerSensor::State &first, const PowerSensor::State &second,
              int pairID) {
  return PowerSensor::Joules(first, second, pairID);
}
}  // namespace

namespace pmt::powersensor2 {

template <typename PowerSensor, typename PowerSensorState>
class PowerSensor2Impl : public PowerSensor2 {
 public:
  PowerSensor2Impl(const char *device)
      : powersensor_(std::make_unique<PowerSensor>(device)),
        firstState_(powersensor_->read()) {}

  State measure() {
    PowerSensorState psState = powersensor_->read();
    State state;
    state.timeAtRead = ::Seconds(firstState_, psState);
    state.joulesAtRead = ::Joules(firstState_, psState, -1);
    return state;
  }

 private:
  virtual const char *getDumpFileName() { return "/tmp/pmt_powersensor2.out"; }

  virtual int getMeasurementInterval() {
    return 1;  // milliseconds
  }

  std::unique_ptr<PowerSensor> powersensor_{};
  PowerSensorState firstState_{};
};

std::unique_ptr<PowerSensor2> PowerSensor2::create(const char *device) {
  return std::make_unique<
      PowerSensor2Impl<PowerSensor::PowerSensor, PowerSensor::State>>(device);
}

}  // end namespace pmt::powersensor2
