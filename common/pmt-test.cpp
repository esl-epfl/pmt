#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "pmt-test.h"

void run(pmt::pmt* sensor, int argc, char* argv[])
{
    char *dumpFileName = std::getenv("PMT_DUMPFILE");
    sensor->startDumpThread(dumpFileName);

    if (argc == 1) {
        auto first = sensor->read();
        while (true) {
            auto start = sensor->read();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto end = sensor->read();
            std::cout << pmt::pmt::seconds(start, end) << " s, ";
            std::cout << pmt::pmt::joules(start, end) << " J, ";
            std::cout << pmt::pmt::watts(start, end )<< " W, ";
            std::cout << pmt::pmt::seconds(first, end) << " s (total), ";
            std::cout << pmt::pmt::joules(first, end) << " J (total), ";
            std::cout << pmt::pmt::watts(first, end )<< " W (average)";
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
        auto start = sensor->read();
        if (system(command.str().c_str()) !=0) {
            perror(command.str().c_str());
        }
        auto end = sensor->read();
        std::cout << "Runtime: " << pmt::pmt::seconds(start, end) << " s" << std::endl;
        std::cout << "Joules: " << pmt::pmt::joules(start, end) << " J" << std::endl;
        std::cout << "Watt: " << pmt::pmt::watts(start, end) << " W" << std::endl;
    }
}
