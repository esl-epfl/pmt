#include "pmt.h"
#include <iostream>

namespace pmt {

pmt::~pmt() { stopDumpThread(); };

double pmt::seconds(const State &firstState, const State &secondState) {
  return secondState.timeAtRead - firstState.timeAtRead;
}

double pmt::joules(const State &firstState, const State &secondState) {
  return secondState.joulesAtRead - firstState.joulesAtRead;
}

double pmt::watts(const State &firstState, const State &secondState) {
  return joules(firstState, secondState) / seconds(firstState, secondState);
}

void pmt::startDumpThread(const char *dumpFileName) {
  if (!dumpFileName) {
    dumpFileName = getDumpFileName();
  }
  dumpFile = std::unique_ptr<std::ofstream>(new std::ofstream(dumpFileName));

  dumpThread = std::thread([&] {
    State startState = read(), currentState = startState;
    previousState = startState;

    while (!stop) {
      std::this_thread::sleep_for(std::chrono::milliseconds(getDumpInterval()));
      currentState = read();
      dump(startState, previousState, currentState);
      previousState = currentState;
    }
  });
}

void pmt::stopDumpThread() {
  stop = true;
  if (dumpThread.joinable()) {
    dumpThread.join();
  }
}

void pmt::dump(const State &startState, const State &firstState,
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

void pmt::mark(const State &startState, const State &currentState,
               const char *name, unsigned tag) const {
  if (dumpFile != nullptr) {
    std::unique_lock<std::mutex> lock(dumpFileMutex);
    *dumpFile << "M " << currentState.timeAtRead - startState.timeAtRead << ' '
              << tag << " \"" << (name == nullptr ? "" : name) << '"'
              << std::endl;
  }
}

double pmt::get_wtime() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() /
         1.0e6;
}

State pmt::read() { return stop ? previousState : measure(); }

}  // end namespace pmt
