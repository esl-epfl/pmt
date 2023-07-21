#include <pybind11/pybind11.h>

#include <../common/pmt.h>

#ifdef BUILD_POWERSENSOR2
#include <../powersensor2/PowerSensor2.h>
#endif

#ifdef BUILD_POWERSENSOR3
#include <../powersensor3/PowerSensor3.h>
#endif

#include <../dummy/Dummy.h>
#include <../tegra/Tegra.h>

#ifdef BUILD_NVML
#include <../nvml/NVML.h>
#endif

#include <../rapl/Rapl.h>

#ifdef BUILD_ROCM
#include <../rocm/ROCM.h>
#endif

#include <../xilinx/Xilinx.h>

namespace py = pybind11;

PYBIND11_MODULE(pypmt, m) {
  m.doc() = "libpmt python bindings";

  py::class_<pmt::PMT>(m, "PMT")
      .def("seconds", &pmt::PMT::seconds, "Get elapsed time")
      .def("joules", &pmt::PMT::joules, "Get energy consumption")
      .def("watts", &pmt::PMT::watts, "Get average power consumption");

  py::class_<pmt::State>(m, "State");

#ifdef BUILD_POWERSENSOR2
  py::class_<pmt::powersensor2::PowerSensor2>(m, "PowerSensor2")
      .def("create", &pmt::powersensor2::PowerSensor2::Create)
      .def("read", &pmt::powersensor2::PowerSensor2::Read)
      .def("startDump", &pmt::powersensor2::PowerSensor2::StartDump)
      .def("stopDump", &pmt::powersensor2::PowerSensor2::StopDump);
#endif

#ifdef BUILD_POWERSENSOR3
  py::class_<pmt::powersensor3::PowerSensor3>(m, "PowerSensor3")
      .def("create", &pmt::powersensor3::PowerSensor3::Create)
      .def("read", &pmt::powersensor3::PowerSensor3::Read)
      .def("startDump", &pmt::powersensor3::PowerSensor3::StartDump)
      .def("stopDump", &pmt::powersensor3::PowerSensor3::StopDump);
#endif

  py::class_<pmt::Dummy>(m, "Dummy")
      .def("create", &pmt::Dummy::Create)
      .def("read", &pmt::Dummy::Read)
      .def("startDump", &pmt::Dummy::StartDump)
      .def("stopDump", &pmt::Dummy::StopDump);

  py::class_<pmt::tegra::Tegra>(m, "tegra")
      .def("create", &pmt::tegra::Tegra::Create)
      .def("read", &pmt::tegra::Tegra::Read)
      .def("startDump", &pmt::tegra::Tegra::StartDump)
      .def("stopDump", &pmt::tegra::Tegra::StopDump);

#ifdef BUILD_NVML
  py::class_<pmt::nvml::NVML>(m, "NVML")
      .def("create", &pmt::nvml::NVML::Create)
      .def("read", &pmt::nvml::NVML::Read)
      .def("startDump", &pmt::nvml::NVML::StartDump)
      .def("StopDump", &pmt::nvml::NVML::StopDump);
#endif

  py::class_<pmt::rapl::Rapl>(m, "Rapl")
      .def("create", &pmt::rapl::Rapl::Create)
      .def("read", &pmt::rapl::Rapl::Read)
      .def("startDump", &pmt::rapl::Rapl::StartDump)
      .def("stopDump", &pmt::rapl::Rapl::StopDump);

#ifdef BUILD_ROCM
  py::class_<pmt::rocm::ROCM>(m, "ROCM")
      .def("create", &pmt::rocm::ROCM::Create)
      .def("read", &pmt::rocm::ROCM::Read)
      .def("startDump", &pmt::rocm::ROCM::StartDump)
      .def("stopDump", &pmt::rocm::ROCM::StopDump);
#endif

  py::class_<pmt::xilinx::Xilinx>(m, "Xilinx")
      .def("create", &pmt::xilinx::Xilinx::Create)
      .def("read", &pmt::xilinx::Xilinx::Read)
      .def("startDump", &pmt::xilinx::Xilinx::StartDump)
      .def("stopDump", &pmt::xilinx::Xilinx::StopDump);
}
