#include "Rapl.h"
#include "RaplImpl.h"

namespace pmt::rapl {

std::unique_ptr<Rapl> Rapl::create() { return std::make_unique<RaplImpl>(); }

std::vector<std::pair<std::string, double>> Rapl::misc(
    const State& firstState, const State& secondState) {
  const double timeElapsed = seconds(firstState, secondState);
  const size_t n = firstState.misc.size();
  assert(n == secondState.misc.size());
  std::vector<std::pair<std::string, double>> result(n);
  for (size_t i = 0; i < n; i++) {
    const std::string name = firstState.misc[i].first;
    const double joules =
        (secondState.misc[i].second - firstState.misc[i].second);
    const double watts = joules / timeElapsed;
    result[i] = {name, watts};
  }
  return result;
}

}  // end namespace pmt::rapl
