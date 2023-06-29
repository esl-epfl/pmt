#include "PowerSensor3.h"

#include POWERSENSOR3_HEADER

namespace {
double Seconds(const PowerSensor3::State &first,
               const PowerSensor3::State &second) {
  return PowerSensor3::seconds(first, second);
}

double Joules(const PowerSensor3::State &first,
              const PowerSensor3::State &second, int pairID) {
  return PowerSensor3::Joules(first, second, pairID);
}
}  // namespace

namespace pmt::powersensor3 {

template <typename PowerSensor, typename PowerSensorState>
class PowerSensor3Impl : public PowerSensor3 {
 public:
  PowerSensor3Impl(const char *device)
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
  virtual const char *getDumpFileName() { return "/tmp/pmt_powersensor3.out"; }

  virtual int getMeasurementInterval() {
    return 1;  // milliseconds
  }

  std::unique_ptr<PowerSensor> powersensor_{};
  PowerSensorState firstState_{};
};

std::unique_ptr<PowerSensor3> PowerSensor3::create(const char *device) {
  return std::make_unique<
      PowerSensor3Impl<::PowerSensor3::PowerSensor, ::PowerSensor3::State>>(
      device);
}

}  // end namespace pmt::powersensor3
