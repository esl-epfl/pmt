#include <pybind11/pybind11.h>

#include <pmt.h>

namespace py = pybind11;

PYBIND11_MODULE(pypmt, m) {
  m.doc() = "libpmt python bindings";

  m.def("create", py::overload_cast<const std::string &>(&pmt::Create),
        py::arg("name"));
  m.def("create", py::overload_cast<const std::string &, int>(&pmt::Create),
        py::arg("name"), py::arg("argument"));
  m.def("create",
        py::overload_cast<const std::string &, const char *>(&pmt::Create),
        py::arg("name"), py::arg("argument"));

  py::class_<pmt::PMT>(m, "PMT")
      .def("seconds", &pmt::PMT::seconds, "Get elapsed time")
      .def("joules", &pmt::PMT::joules, "Get energy consumption")
      .def("watts", &pmt::PMT::watts, "Get average power consumption")
      .def("read", &pmt::PMT::Read)
      .def("startDump", &pmt::PMT::StartDump)
      .def("stopDump", &pmt::PMT::StopDump);

  py::class_<pmt::State>(m, "State");
}
