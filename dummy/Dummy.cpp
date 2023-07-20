#include "Dummy.h"

namespace pmt {

class DummyImpl : public Dummy {
 private:
  virtual State GetState() override;

  virtual const char *GetDumpFilename() { return nullptr; }

  virtual int GetMeasurementInterval() { return 0; }
};

std::unique_ptr<Dummy> Dummy::Create() { return std::make_unique<DummyImpl>(); }

State DummyImpl::GetState() {
  State state;
  state.timestamp_ = PMT::GetTime();
  state.name_[0] = "none";
  state.joules_[0] = 0;
  state.watt_[0] = 0;
  return state;
}

}  // end namespace pmt
