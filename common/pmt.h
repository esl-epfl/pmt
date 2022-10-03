#ifndef PMT_H_
#define PMT_H_

#include "pmt-config.h"

#include <fstream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace pmt {

class State {
public:
  double timeAtRead;
  double joulesAtRead;
  std::vector<double> misc;
};

class pmt {
public:
  virtual ~pmt();

  virtual State read();

  static double seconds(const State &firstState, const State &secondState);

  static double joules(const State &firstState, const State &secondState);

  static double watts(const State &firstState, const State &secondState);

  void startDumpThread(const char *dumpFileName);
  void stopDumpThread();

  void mark(const State &startState, const State &currentState,
            const char *name = 0, unsigned tag = 0) const;

protected:
  virtual State measure() = 0;

  virtual const char *getDumpFileName() = 0;

  virtual int getDumpInterval() = 0;

  void dump(const State &startState, const State &firstState,
            const State &secondState);

  static double get_wtime();

private:
  std::thread dumpThread;
  std::unique_ptr<std::ofstream> dumpFile = nullptr;
  mutable std::mutex dumpFileMutex;
  volatile bool stop = false;
  State previousState;
};
} // end namespace pmt

#endif
