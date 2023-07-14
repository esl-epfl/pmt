project(python)

add_library(pypmt MODULE pypmt.cpp)

target_link_libraries(
  pypmt
  PRIVATE pybind11::module
          pybind11::lto
          pybind11::windows_extras
          $<TARGET_OBJECTS:pmt-common>
          pmt-dummy
          pmt-tegra
          pmt-rapl
          pmt-xilinx)

if(${BUILD_POWERSENSOR2_PMT})
  add_definitions(-DBUILD_POWERSENSOR2)
  target_link_libraries(pypmt PRIVATE pmt-powersensor2)
endif()

if(${BUILD_POWERSENSOR3_PMT})
  add_definitions(-DBUILD_POWERSENSOR3)
  target_link_libraries(pypmt PRIVATE pmt-powersensor3)
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
  FILES PythonNVMLDemo.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

install(
  FILES pmt.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python")
