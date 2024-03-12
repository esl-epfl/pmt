project(python)

add_library(pypmt MODULE pypmt.cpp)
target_include_directories(pypmt PRIVATE ${CMAKE_BINARY_DIR})

set(LINK_LIBRARIES pmt-dummy)
if(PMT_BUILD_CRAY)
  list(APPEND LINK_LIBRARIES pmt-cray)
endif()
if(PMT_BUILD_RAPL)
  list(APPEND LINK_LIBRARIES pmt-rapl)
endif()
if(PMT_BUILD_TEGRA)
  list(APPEND LINK_LIBRARIES pmt-tegra)
endif()
if(PMT_BUILD_XILINX)
  list(APPEND LINK_LIBRARIES pmt-xilinx)
endif()

target_link_libraries(
  pypmt PRIVATE pybind11::module pybind11::lto pybind11::windows_extras
                $<TARGET_OBJECTS:pmt-common> ${LINK_LIBRARIES})

if(${PMT_BUILD_POWERSENSOR2})
  target_link_libraries(pypmt PRIVATE pmt-powersensor2)
endif()

if(${PMT_BUILD_POWERSENSOR3})
  target_link_libraries(pypmt PRIVATE pmt-powersensor3)
endif()

if(${PMT_BUILD_NVML})
  target_link_libraries(pypmt PRIVATE pmt-nvml)
endif()

if(${PMT_BUILD_ROCM})
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
