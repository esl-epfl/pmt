#include "Arduinopmt.h"

/*
    Include original PowerSensor header file
    The _ prefixed aliases prevents class and
    namespace conflicts.
*/
#include POWERSENSOR_HEADER
using _PowerSensor = PowerSensor::PowerSensor;
using _State = PowerSensor::State;
double (&_seconds)(const _State &, const _State &) = PowerSensor::seconds;
double (&_Joules)(const _State &, const _State &, int) = PowerSensor::Joules;

namespace pmt {
namespace arduino {

class Arduinopmt_ : public Arduinopmt {
public:
  Arduinopmt_(const char *device);
  ~Arduinopmt_();

  State measure();

private:
  virtual const char *getDumpFileName() { return "/tmp/arduinopmt.out"; }

  virtual int getDumpInterval() {
    return 1; // milliseconds
  }

  _PowerSensor *_powersensor;
  _State _firstState;
};

Arduinopmt *Arduinopmt::create(const char *device) {
  return new Arduinopmt_(device);
}

Arduinopmt_::Arduinopmt_(const char *device) {
  _powersensor = new _PowerSensor(device);
  _firstState = _powersensor->read();
}

Arduinopmt_::~Arduinopmt_() { delete _powersensor; }

State Arduinopmt_::measure() {
  _State _state = _powersensor->read();
  State state;
  state.timeAtRead = _seconds(_firstState, _state);
  state.joulesAtRead = _Joules(_firstState, _state, -1);
  return state;
}

} // end namespace arduino
} // end namespace pmt
