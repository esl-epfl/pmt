#include <cassert>
#include <iostream>
#include <iomanip>

#include "pmt.h"
namespace pmt {

PMT::~PMT() { stopDumpThread(); };

double PMT::seconds(const State &firstState, const State &secondState) {
  return secondState.timeAtRead - firstState.timeAtRead;
}

double PMT::joules(const State &firstState, const State &secondState) {
  return secondState.joulesAtRead - firstState.joulesAtRead;
}

double PMT::watts(const State &firstState, const State &secondState) {
  return joules(firstState, secondState) / seconds(firstState, secondState);
}

std::vector<std::pair<std::string, double>> PMT::misc(
    const State &firstState, const State &secondState) {
  std::vector<std::pair<std::string, double>> result;
  return result;
}

float PMT::getDumpInterval() {
  const char *dumpIntervalStr = std::getenv(kDumpIntervalVariable.c_str());
  return dumpIntervalStr ? std::stoi(dumpIntervalStr)
                         : static_cast<float>(getMeasurementInterval()) * 1e-3;
}

void PMT::startDumpThread(const char *dumpFileName) {
  if (!dumpFileName) {
    dumpFileName = getDumpFileName();
  }
  assert(dumpFileName);
  dumpFile = std::make_unique<std::ofstream>(dumpFileName);

  dumpThread = std::thread([&] {
    const int measurementInterval =
        getMeasurementInterval();                  // in milliseconds
    const float dumpInterval = getDumpInterval();  // in seconds
    assert(dumpInterval);

    const State startState = read();
    State currentState = startState;
    previousState = startState;
    State dumpState = startState;

    while (!stop) {
      assert(dumpInterval > 0);
      std::this_thread::sleep_for(
          std::chrono::milliseconds(measurementInterval));
      currentState = read();
      const float elapsedTime = seconds(dumpState, currentState);
      if (elapsedTime > dumpInterval) {
        dump(startState, dumpState, currentState);
        dumpState = currentState;
        previousState = currentState;
      }
    }
  });
}

void PMT::stopDumpThread() {
  stop = true;
  if (dumpThread.joinable()) {
    dumpThread.join();
  }
}

void PMT::dump(const State &startState, const State &firstState,
               const State &secondState) {
  if (dumpFile != nullptr) {
    std::unique_lock<std::mutex> lock(dumpFileMutex);
    *dumpFile << "S " << seconds(startState, secondState) << " " << std::fixed
              << std::setprecision(3) << watts(firstState, secondState);
    for (const std::pair<std::string, double> &m :
         misc(firstState, secondState)) {
      if (m.first.empty()) {
        *dumpFile << " " << m.second;
      } else {
        *dumpFile << " " << m.first << ":" << m.second;
      }
    }
    *dumpFile << std::endl;
  }
}

void PMT::mark(const State &startState, const State &currentState,
               const char *name, unsigned tag) const {
  if (dumpFile != nullptr) {
    std::unique_lock<std::mutex> lock(dumpFileMutex);
    *dumpFile << "M " << currentState.timeAtRead - startState.timeAtRead << ' '
              << tag << " \"" << (name == nullptr ? "" : name) << '"'
              << std::endl;
  }
}

double PMT::get_wtime() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() /
         1.0e6;
}

}  // end namespace pmt
