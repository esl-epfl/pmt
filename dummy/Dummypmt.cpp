#include "Dummypmt.h"

namespace pmt {

class Dummypmt_ : public Dummypmt {
 private:
  virtual State measure();

  virtual const char *getDumpFileName() { return nullptr; }

  virtual int getDumpInterval() { return 0; }
};

Dummypmt *Dummypmt::create() { return new Dummypmt_(); }

State Dummypmt_::measure() {
  State state;
  state.timeAtRead = pmt::get_wtime();
  state.joulesAtRead = 0;
  return state;
}

}  // end namespace pmt
