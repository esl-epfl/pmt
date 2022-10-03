#include "Raplpmt.h"
#include "rapl-read.h"

namespace pmt {
namespace rapl {

class Raplpmt_ : public Raplpmt {
private:
  virtual State measure();

  virtual const char *getDumpFileName() { return "/tmp/raplpmt.out"; }

  virtual int getDumpInterval() {
    return 500; // milliseconds
  }

  Rapl rapl;
};

Raplpmt *Raplpmt::create() { return new Raplpmt_(); }

State Raplpmt_::measure() {
  State state;
  state.timeAtRead = get_wtime();
  state.joulesAtRead = rapl.measure();
  return state;
}

} // end namespace rapl
} // end namespace pmt
