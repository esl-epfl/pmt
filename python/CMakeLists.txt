project(python)

add_library(pypmt MODULE pypmt.cpp)
target_include_directories(pypmt PRIVATE ${CMAKE_BINARY_DIR})

set(LINK_LIBRARIES pmt-dummy)
if(BUILD_CRAY_PMT)
  list(APPEND LINK_LIBRARIES pmt-cray)
  add_definitions(-DBUILD_CRAY)
endif()
if(BUILD_RAPL_PMT)
  list(APPEND LINK_LIBRARIES pmt-rapl)
  add_definitions(-DBUILD_RAPL)
endif()
if(BUILD_TEGRA_PMT)
  list(APPEND LINK_LIBRARIES pmt-tegra)
  add_definitions(-DBUILD_TEGRA)
endif()
if(BUILD_XILINX_PMT)
  list(APPEND LINK_LIBRARIES pmt-xilinx)
  add_definitions(-DBUILD_XILINX)
endif()

target_link_libraries(
  pypmt PRIVATE pybind11::module pybind11::lto pybind11::windows_extras
                $<TARGET_OBJECTS:pmt-common> ${LINK_LIBRARIES})

if(${BUILD_POWERSENSOR2_PMT})
  add_definitions(-DBUILD_POWERSENSOR2)
  target_link_libraries(pypmt PRIVATE pmt-powersensor2)
endif()

if(${BUILD_POWERSENSOR3_PMT})
  add_definitions(-DBUILD_POWERSENSOR3)
  target_link_libraries(pypmt PRIVATE pmt-powersensor3)
endif()

if(${BUILD_NVML_PMT})
  add_definitions(-DBUILD_NVML)
  target_link_libraries(pypmt PRIVATE pmt-nvml)
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

install(TARGETS pypmt DESTINATION "${CMAKE_INSTALL_LIBDIR}/python")

install(
  FILES PythonNVMLDemo.py
  COMPONENT python
  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
  FILES pmt.py
  COMPONENT python
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/python")
