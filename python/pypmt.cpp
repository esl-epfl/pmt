#include <pybind11/pybind11.h>

#include <../common/pmt.h>

#ifdef BUILD_POWERSENSOR2
#include <../powersensor2/PowerSensor2.h>
#endif

#ifdef BUILD_POWERSENSOR3
#include <../powersensor3/PowerSensor3.h>
#endif

#include <../dummy/Dummy.h>
#include <../jetson/Jetson.h>

#ifdef BUILD_LIKWID
#include <../likwid/Likwid.h>
#endif

#ifdef BUILD_NVML
#include <../nvml/NVML.h>
#endif

#include <../rapl/Rapl.h>

#ifdef BUILD_ROCM
#include <../rocm/ROCM.h>
#endif

#include <../xilinx/Xilinx.h>

namespace py = pybind11;

double seconds(pmt::State start, pmt::State end) {
  return end.timeAtRead - start.timeAtRead;
}

double joules(pmt::State start, pmt::State end) {
  return end.joulesAtRead - start.joulesAtRead;
}

double watts(pmt::State start, pmt::State end) {
  return joules(start, end) / seconds(start, end);
}

PYBIND11_MODULE(pypmt, m) {
  m.doc() = "libpmt python bindings";

  m.def("seconds", &seconds, "Get elapsed time");
  m.def("joules", &joules, "Get energy consumption");
  m.def("watts", &watts, "Get average power consumption");

  py::class_<pmt::State>(m, "State");

#ifdef BUILD_POWERSENSOR2
  py::class_<pmt::powersensor2::PowerSensor2>(m, "PowerSensor2")
      .def("create", &pmt::powersensor2::PowerSensor2::create)
      .def("read", &pmt::powersensor2::PowerSensor2::read)
      .def("startDumpThread", &pmt::powersensor2::PowerSensor2::startDumpThread)
      .def("stopDumpThread", &pmt::powersensor2::PowerSensor2::stopDumpThread);
#endif

#ifdef BUILD_POWERSENSOR3
  py::class_<pmt::powersensor3::PowerSensor3>(m, "PowerSensor3")
      .def("create", &pmt::powersensor3::PowerSensor3::create)
      .def("read", &pmt::powersensor3::PowerSensor3::read)
      .def("startDumpThread", &pmt::powersensor3::PowerSensor3::startDumpThread)
      .def("stopDumpThread", &pmt::powersensor3::PowerSensor3::stopDumpThread);
#endif

  py::class_<pmt::Dummy>(m, "Dummy")
      .def("create", &pmt::Dummy::create)
      .def("read", &pmt::Dummy::read)
      .def("startDumpThread", &pmt::Dummy::startDumpThread)
      .def("stopDumpThread", &pmt::Dummy::stopDumpThread);

  py::class_<pmt::jetson::Jetson>(m, "Jetson")
      .def("create", &pmt::jetson::Jetson::create)
      .def("read", &pmt::jetson::Jetson::read)
      .def("startDumpThread", &pmt::jetson::Jetson::startDumpThread)
      .def("stopDumpThread", &pmt::jetson::Jetson::stopDumpThread);

#ifdef BUILD_LIKWID
  py::class_<pmt::likwid::Likwid>(m, "Likwid")
      .def("create", &pmt::likwid::Likwid::create)
      .def("read", &pmt::likwid::Likwid::read)
      .def("startDumpThread", &pmt::likwid::Likwid::startDumpThread)
      .def("stopDumpThread", &pmt::likwid::Likwid::stopDumpThread);
#endif

#ifdef BUILD_NVML
  py::class_<pmt::nvml::NVML>(m, "NVML")
      .def("create", &pmt::nvml::NVML::create)
      .def("read", &pmt::nvml::NVML::read)
      .def("startDumpThread", &pmt::nvml::NVML::startDumpThread)
      .def("stopDumpThread", &pmt::nvml::NVML::stopDumpThread);
#endif

  py::class_<pmt::rapl::Rapl>(m, "Rapl")
      .def("create", &pmt::rapl::Rapl::create)
      .def("read", &pmt::rapl::Rapl::read)
      .def("startDumpThread", &pmt::rapl::Rapl::startDumpThread)
      .def("stopDumpThread", &pmt::rapl::Rapl::stopDumpThread);

#ifdef BUILD_ROCM
  py::class_<pmt::rocm::ROCM>(m, "ROCM")
      .def("create", &pmt::rocm::ROCM::create)
      .def("read", &pmt::rocm::ROCM::read)
      .def("startDumpThread", &pmt::rocm::ROCM::startDumpThread)
      .def("stopDumpThread", &pmt::rocm::ROCM::stopDumpThread);
#endif

  py::class_<pmt::xilinx::Xilinx>(m, "Xilinx")
      .def("create", &pmt::xilinx::Xilinx::create)
      .def("read", &pmt::xilinx::Xilinx::read)
      .def("startDumpThread", &pmt::xilinx::Xilinx::startDumpThread)
      .def("stopDumpThread", &pmt::xilinx::Xilinx::stopDumpThread);
}
