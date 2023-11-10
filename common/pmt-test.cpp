#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

#include "pmt.h"

void run(pmt::PMT &sensor, int argc, char *argv[]) {
  const char *filename = std::getenv(pmt::kDumpFilenameVariable.c_str());
  sensor.StartDump(filename);

  if (argc == 1) {
    auto first = sensor.Read();
    while (true) {
      auto start = sensor.Read();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(sensor.GetMeasurementInterval()));
      auto end = sensor.Read();
      std::cout << std::fixed << std::setprecision(3);
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
    auto start = sensor.Read();
    if (system(command.str().c_str()) != 0) {
      perror(command.str().c_str());
    }
    auto end = sensor.Read();
    std::cout << "Runtime: " << pmt::PMT::seconds(start, end) << " s"
              << std::endl;
    std::cout << "Joules: " << pmt::PMT::joules(start, end) << " J"
              << std::endl;
    std::cout << "Watt: " << pmt::PMT::watts(start, end) << " W" << std::endl;
  }
}
