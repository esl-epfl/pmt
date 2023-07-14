#include <fstream>
#include <iostream>

#include <omp.h>
#include <unistd.h>

#include "Tegra.h"
#include "TegraImpl.h"

namespace pmt::tegra {

std::unique_ptr<Tegra> Tegra::create() { return std::make_unique<TegraImpl>(); }

std::vector<std::pair<std::string, double>> Tegra::misc(
    const State &firstState, const State &secondState) {
  const size_t n = secondState.misc.size();
  std::vector<std::pair<std::string, double>> result(n);
  for (size_t i = 0; i < n; i++) {
    const std::string &name = secondState.misc[i].first;
    const double watts = secondState.misc[i].second;
    result[i] = {name, watts};
  }
  return result;
}

}  // end namespace pmt::tegra
