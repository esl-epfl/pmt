#include "Rapl.h"
#include "rapl-read.h"

namespace pmt {
namespace rapl {

class Rapl_ : public Rapl {
 private:
  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/raplpmt.out"; }

  virtual int getDumpInterval() {
    return 500;  // milliseconds
  }

  raplread::Rapl rapl;
};

Rapl *Rapl::create() { return new Rapl_(); }

State Rapl_::measure() {
  State state;
  state.timeAtRead = get_wtime();
  state.joulesAtRead = rapl.measure();
  return state;
}

}  // end namespace rapl
}  // end namespace pmt
