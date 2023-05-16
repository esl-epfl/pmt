#include <cassert>
#include <iostream>

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
      if (stop || (elapsedTime > dumpInterval)) {
        dump(startState, dumpState, currentState);
        dumpState = currentState;
      }
      previousState = currentState;
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
    *dumpFile << "S " << seconds(startState, secondState) << " "
              << watts(firstState, secondState);
    for (int i = 0; i < secondState.misc.size(); i++) {
      if (i > 0) {
        *dumpFile << " ";
      }
      *dumpFile << secondState.misc[i];
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

State PMT::read() { return stop ? previousState : measure(); }

}  // end namespace pmt
