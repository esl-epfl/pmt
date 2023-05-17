#include "Arduino.h"

/*
    Include original PowerSensor header file
    The _ prefixed aliases prevents class and
    namespace conflicts.
*/
#include POWERSENSOR2_HEADER
#include POWERSENSOR3_HEADER

double Seconds_(const PowerSensor::State &first,
                const PowerSensor::State &second) {
  return PowerSensor::seconds(first, second);
}
double Seconds_(const PowerSensor3::State &first,
                const PowerSensor3::State &second) {
  return PowerSensor3::seconds(first, second);
}

double Joules_(const PowerSensor::State &first,
               const PowerSensor::State &second, int pairID) {
  return PowerSensor::Joules(first, second, pairID);
}
double Joules_(const PowerSensor3::State &first,
               const PowerSensor3::State &second, int pairID) {
  return PowerSensor3::Joules(first, second, pairID);
}

namespace pmt::arduino {

template <typename PowerSensor, typename PowerSensorState>
class ArduinoImpl : public Arduino {
 public:
  ArduinoImpl(const char *device) {
    powersensor_ = new PowerSensor(device);
    firstState_ = powersensor_->read();
  }
  ~ArduinoImpl() { delete powersensor_; };

  State measure() {
    PowerSensorState psState = powersensor_->read();
    State state;
    state.timeAtRead = Seconds_(firstState_, psState);
    state.joulesAtRead = Joules_(firstState_, psState, -1);
    return state;
  }

 private:
  virtual const char *getDumpFileName() { return "/tmp/pmt_arduino.out"; }

  virtual int getMeasurementInterval() {
    return 1;  // milliseconds
  }

  PowerSensor *powersensor_{};
  PowerSensorState firstState_{};
};

std::unique_ptr<Arduino> Arduino::create(const char *device,
                                         POWERSENSOR_VERSION version) {
  if (version == V2) {
    return std::make_unique<
        ArduinoImpl<PowerSensor::PowerSensor, PowerSensor::State>>(device);
  } else if (version == V3) {
    return std::make_unique<
        ArduinoImpl<PowerSensor3::PowerSensor, PowerSensor3::State>>(device);
  } else {
    throw std::invalid_argument("Unknown PowerSensor version " +
                                std::to_string(version));
  }
}

}  // end namespace pmt::arduino
