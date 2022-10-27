#include "../common/pmt-test.h"

#include "Jetson.h"

#include <signal.h>

pmt::jetson::Jetson* sensor;

void signal_callback_handler(int signum) {
  delete sensor;
  exit(signum);
}

int main(int argc, char** argv) {
  signal(SIGINT, signal_callback_handler);
  sensor = pmt::jetson::Jetson::create();
  run(sensor, argc, argv);
}
