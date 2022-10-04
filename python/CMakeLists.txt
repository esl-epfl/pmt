project(python)

add_library(pypmt MODULE pypmt.cpp)

target_link_libraries(
  pypmt
  PRIVATE pybind11::module
          pybind11::lto
          pybind11::windows_extras
          $<TARGET_OBJECTS:pmt-common>
          pmt-amdgpu
          pmt-dummy
          pmt-jetson
          pmt-rapl
          pmt-xilinx)

if(${BUILD_ARDUINO_PMT})
  add_definitions(-DBUILD_ARDUINO)
  target_link_libraries(pypmt PRIVATE pmt-arduino)
endif()

if(${LIKWID_FOUND})
  add_definitions(-DBUILD_LIKWID)
  target_link_libraries(pypmt PRIVATE pmt-likwid)
endif()

if(${BUILD_NVML_PMT})
  find_package(CUDAToolkit QUIET)
  if(${CUDAToolkit_FOUND})
    add_definitions(-DBUILD_NVML)
    target_link_libraries(pypmt PRIVATE pmt-nvml)
  endif()
endif()

if(${ROCM-SMI_FOUND})
  add_definitions(-DBUILD_ROCM)
  target_link_libraries(pypmt PRIVATE pmt-rocm)
endif()

pybind11_extension(pypmt)
if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
  # Strip unnecessary sections of the binary on Linux/macOS
  pybind11_strip(pypmt)
endif()

set_target_properties(pypmt PROPERTIES CXX_VISIBILITY_PRESET "hidden"
                                       CUDA_VISIBILITY_PRESET "hidden")

install(TARGETS pypmt DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python")

install(
  FILES PythonNVMLpmtDemo.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

install(
  FILES pmt_decorator.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python")
