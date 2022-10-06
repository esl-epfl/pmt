#include "Dummy.h"

namespace pmt {

class Dummy_ : public Dummy {
 private:
  virtual State measure();

  virtual const char *getDumpFileName() { return nullptr; }

  virtual int getDumpInterval() { return 0; }
};

Dummy *Dummy::create() { return new Dummy_(); }

State Dummy_::measure() {
  State state;
  state.timeAtRead = pmt::get_wtime();
  state.joulesAtRead = 0;
  return state;
}

}  // end namespace pmt
