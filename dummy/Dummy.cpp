#include "Dummy.h"

namespace pmt {

class Dummy_ : public Dummy {
 private:
  virtual State measure();

  virtual const char *getDumpFileName() { return nullptr; }

  virtual int getMeasurementInterval() { return 0; }
};

std::unique_ptr<Dummy> Dummy::create() { return std::make_unique<Dummy_>(); }

State Dummy_::measure() {
  State state;
  state.timeAtRead = PMT::get_wtime();
  state.joulesAtRead = 0;
  return state;
}

}  // end namespace pmt
