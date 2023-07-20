#include <cassert>
#include <iomanip>

#include "pmt.h"
namespace pmt {

PMT::~PMT() {
  StopDump();
  StopThread();
};

double PMT::seconds(const State &first, const State &second) {
  return second.timestamp_ - first.timestamp_;
}

double PMT::joules(const State &first, const State &second) {
  return second.joules_[0] - first.joules_[0];
}

double PMT::watts(const State &first, const State &second) {
  return joules(first, second) / seconds(first, second);
}

float PMT::GetDumpInterval() {
  const char *dump_interval_ = std::getenv(kDumpIntervalVariable.c_str());
  return dump_interval_ ? std::stoi(dump_interval_)
                        : static_cast<float>(GetMeasurementInterval()) * 1e-3;
}

void PMT::StartThread() {
  thread_ = std::thread([&] {
    const State start = GetState();
    assert(start.nr_measurements_ > 0);
    State previous = start;
    state_latest_ = start;

    const int measurement_interval =
        GetMeasurementInterval();  // in milliseconds
    assert(measurement_interval > 0);
    const float dumpInterval = GetDumpInterval();  // in seconds
    assert(dumpInterval > 0);

    while (!thread_stop_) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(measurement_interval));
      state_latest_ = GetState();

      const float duration = seconds(previous, state_latest_);
      if (dump_file_ && duration > dumpInterval) {
        assert(state_latest_.nr_measurements_ > 0);
        assert(start.nr_measurements_ > 0);
        assert(previous.nr_measurements_ > 0);
        Dump(start, previous, state_latest_);
        previous = state_latest_;
      }
    }
  });
}

void PMT::StopThread() {
  thread_stop_ = true;
  if (thread_.joinable()) {
    thread_.join();
  }
}

void PMT::StartDump(const char *filename) {
  if (!filename) {
    filename = GetDumpFilename();
  }
  assert(filename);
  dump_file_ = std::make_unique<std::ofstream>(filename);
}

void PMT::StopDump() { dump_file_.reset(); }

void PMT::Dump(const State &start, const State &first, const State &second) {
  if (dump_file_ != nullptr) {
    std::unique_lock<std::mutex> lock(dump_file_mutex_);
    *dump_file_ << "S " << seconds(start, second) << " " << std::fixed
                << std::setprecision(3) << watts(first, second);
    for (size_t i = 0; i < second.nr_measurements_; i++) {
      const std::string name = second.name_[i];
      const float watt = second.watt_[i];
      if (name.empty()) {
        *dump_file_ << " " << watt;
      } else {
        *dump_file_ << " " << name << ":" << watt;
      }
    }
    *dump_file_ << std::endl;
  }
}

void PMT::Mark(const State &start, const State &current, const char *name,
               unsigned tag) const {
  if (dump_file_ != nullptr) {
    std::unique_lock<std::mutex> lock(dump_file_mutex_);
    *dump_file_ << "M " << current.timestamp_ - start.timestamp_ << ' ' << tag
                << " \"" << (name == nullptr ? "" : name) << '"' << std::endl;
  }
}

double PMT::GetTime() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() /
         1.0e6;
}

State PMT::Read() {
  if (!thread_started_) {
    StartThread();
    thread_started_ = true;
  }
  while (seconds(state_previous_, state_latest_) == 0) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(GetMeasurementInterval()));
  }
  state_previous_ = state_latest_;
  return state_latest_;
}

}  // end namespace pmt
