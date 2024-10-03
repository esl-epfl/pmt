#include <cstring>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>

#include <pmt.h>

void run(pmt::PMT& sensor, int argc, char* argv[]) {
  const char* filename = std::getenv(pmt::kDumpFilenameVariable.c_str());
  sensor.StartDump(filename);

  if (argc == 1) {
    auto first = sensor.Read();
    while (true) {
      auto state = sensor.Read();
      std::cout << pmt::PMT::seconds(first, state) << " s, ";
      for (int i = 0; i < state.NrMeasurements(); i++) {
        std::cout << state.name(i) << ": " << state.watts(i) << " W";
        if (i < (state.NrMeasurements() - 1)) {
          std::cout << ", ";
        }
      }
      std::cout << std::endl;
      std::this_thread::sleep_for(
          std::chrono::milliseconds(sensor.GetMeasurementInterval()));
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

int main(int argc, char* argv[]) {
  try {
    const std::string pmt_name_env = "PMT_NAME";
    const std::string pmt_device_env = "PMT_DEVICE";
    const char* pmt_name = std::getenv(pmt_name_env.c_str());
    const char* pmt_device = std::getenv(pmt_device_env.c_str());
    if (pmt_name == nullptr) {
      throw std::runtime_error(
          "Select PMT using the PMT_NAME environment variable.");
    } else {
      std::unique_ptr<pmt::PMT> sensor = pmt::Create(pmt_name, pmt_device);
      run(*sensor, argc, argv);
    }
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what();
    return EXIT_FAILURE;
  }
}