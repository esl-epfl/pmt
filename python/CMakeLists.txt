project(python)

add_library(pypmt MODULE pypmt.cpp)
target_include_directories(pypmt PRIVATE ${PMT_INCLUDE_DIR})

target_link_libraries(pypmt PRIVATE pybind11::module pybind11::lto
                                    pybind11::windows_extras pmt)

pybind11_extension(pypmt)
if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
  # Strip unnecessary sections of the binary on Linux/macOS
  pybind11_strip(pypmt)
endif()

set_target_properties(pypmt PROPERTIES CXX_VISIBILITY_PRESET "hidden"
                                       CUDA_VISIBILITY_PRESET "hidden")

install(TARGETS pypmt DESTINATION "${CMAKE_INSTALL_LIBDIR}/python")

install(
  FILES PythonNVMLDemo.py
  COMPONENT python
  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
  FILES pmt.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/python")
