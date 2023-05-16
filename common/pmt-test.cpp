#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "pmt-test.h"

void run(pmt::PMT &sensor, int argc, char *argv[]) {
  const char *dumpFileName = std::getenv(pmt::kDumpFilenameVariable.c_str());
  sensor.startDumpThread(dumpFileName);

  if (argc == 1) {
    auto first = sensor.read();
    while (true) {
      auto start = sensor.read();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      auto end = sensor.read();
      std::cout << pmt::PMT::seconds(start, end) << " s, ";
      std::cout << pmt::PMT::joules(start, end) << " J, ";
      std::cout << pmt::PMT::watts(start, end) << " W, ";
      std::cout << pmt::PMT::seconds(first, end) << " s (total), ";
      std::cout << pmt::PMT::joules(first, end) << " J (total), ";
      std::cout << pmt::PMT::watts(first, end) << " W (average)";
      std::cout << std::endl;
    }
  } else {
    std::stringstream command;
    for (int i = 1; i < argc; i++) {
      if (i > 1) {
        command << " ";
      }
      command << argv[i];
    }
    auto start = sensor.read();
    if (system(command.str().c_str()) != 0) {
      perror(command.str().c_str());
    }
    auto end = sensor.read();
    std::cout << "Runtime: " << pmt::PMT::seconds(start, end) << " s"
              << std::endl;
    std::cout << "Joules: " << pmt::PMT::joules(start, end) << " J"
              << std::endl;
    std::cout << "Watt: " << pmt::PMT::watts(start, end) << " W" << std::endl;
  }
}
