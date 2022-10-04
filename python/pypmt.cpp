#include <pybind11/pybind11.h>

#include <../amdgpu/AMDGPU.h>
#include <../common/pmt.h>

#ifdef BUILD_ARDUINO
#include <../arduino/Arduino.h>
#endif

#include <../dummy/Dummypmt.h>
#include <../jetson/Jetsonpmt.h>

#ifdef BUILD_LIKWID
#include <../likwid/Likwidpmt.h>
#endif

#ifdef BUILD_NVML
#include <../nvml/NVMLpmt.h>
#endif

#include <../rapl/Raplpmt.h>

#ifdef BUILD_ROCM
#include <../rocm/ROCMpmt.h>
#endif

#include <../xilinx/Xilinxpmt.h>

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

  py::class_<pmt::amdgpu::AMDGPU>(m, "AMDGPU")
      .def("create", &pmt::amdgpu::AMDGPU::create)
      .def("read", &pmt::amdgpu::AMDGPU::read)
      .def("startDumpThread", &pmt::amdgpu::AMDGPU::startDumpThread)
      .def("stopDumpThread", &pmt::amdgpu::AMDGPU::stopDumpThread);

#ifdef BUILD_ARDUINO
  py::class_<pmt::arduino::Arduino>(m, "Arduino")
      .def("create", &pmt::arduino::Arduino::create)
      .def("read", &pmt::arduino::Arduino::read)
      .def("startDumpThread", &pmt::arduino::Arduino::startDumpThread)
      .def("stopDumpThread", &pmt::arduino::Arduino::stopDumpThread);
#endif

  py::class_<pmt::Dummypmt>(m, "Dummypmt")
      .def("create", &pmt::Dummypmt::create)
      .def("read", &pmt::Dummypmt::read)
      .def("startDumpThread", &pmt::Dummypmt::startDumpThread)
      .def("stopDumpThread", &pmt::Dummypmt::stopDumpThread);

  py::class_<pmt::jetson::Jetsonpmt>(m, "Jetsonpmt")
      .def("create", &pmt::jetson::Jetsonpmt::create)
      .def("read", &pmt::jetson::Jetsonpmt::read)
      .def("startDumpThread", &pmt::jetson::Jetsonpmt::startDumpThread)
      .def("stopDumpThread", &pmt::jetson::Jetsonpmt::stopDumpThread);

#ifdef BUILD_LIKWID
  py::class_<pmt::likwid::Likwidpmt>(m, "Likwidpmt")
      .def("create", &pmt::likwid::Likwidpmt::create)
      .def("read", &pmt::likwid::Likwidpmt::read)
      .def("startDumpThread", &pmt::likwid::Likwidpmt::startDumpThread)
      .def("stopDumpThread", &pmt::likwid::Likwidpmt::stopDumpThread);
#endif

#ifdef BUILD_NVML
  py::class_<pmt::nvml::NVMLpmt>(m, "NVMLpmt")
      .def("create", &pmt::nvml::NVMLpmt::create)
      .def("read", &pmt::nvml::NVMLpmt::read)
      .def("startDumpThread", &pmt::nvml::NVMLpmt::startDumpThread)
      .def("stopDumpThread", &pmt::nvml::NVMLpmt::stopDumpThread);
#endif

  py::class_<pmt::rapl::Raplpmt>(m, "Raplpmt")
      .def("create", &pmt::rapl::Raplpmt::create)
      .def("read", &pmt::rapl::Raplpmt::read)
      .def("startDumpThread", &pmt::rapl::Raplpmt::startDumpThread)
      .def("stopDumpThread", &pmt::rapl::Raplpmt::stopDumpThread);

#ifdef BUILD_ROCM
  py::class_<pmt::rocm::ROCMpmt>(m, "ROCMpmt")
      .def("create", &pmt::rocm::ROCMpmt::create)
      .def("read", &pmt::rocm::ROCMpmt::read)
      .def("startDumpThread", &pmt::rocm::ROCMpmt::startDumpThread)
      .def("stopDumpThread", &pmt::rocm::ROCMpmt::stopDumpThread);
#endif

  py::class_<pmt::xilinx::Xilinxpmt>(m, "Xilinxpmt")
      .def("create", &pmt::xilinx::Xilinxpmt::create)
      .def("read", &pmt::xilinx::Xilinxpmt::read)
      .def("startDumpThread", &pmt::xilinx::Xilinxpmt::startDumpThread)
      .def("stopDumpThread", &pmt::xilinx::Xilinxpmt::stopDumpThread);
}
