project(pmt)

cmake_minimum_required(VERSION 3.17.5)

# Set cmake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Set build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Default build type." FORCE)
endif()

#Set CXX Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set debug print info
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Enable OpenMP
find_package(OpenMP REQUIRED)

# Enable PThreads
find_package(Threads REQUIRED)

# Enable compile commands (needed for iwyu)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add PMT source directory as include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Options to disable implementations
# that require external dependencies
option(BUILD_CRAY_PMT "" OFF)
option(BUILD_LIKWID_PMT "" OFF)
option(BUILD_NVML_PMT "" OFF)
option(BUILD_POWERSENSOR2_PMT "" OFF)
option(BUILD_POWERSENSOR3_PMT "" OFF)
option(BUILD_PYTHON_PMT "" OFF)
option(BUILD_RAPL_PMT "" OFF)
option(BUILD_ROCM_PMT "" OFF)
option(BUILD_TEGRA_PMT "" OFF)
option(BUILD_XILINX_PMT "" OFF)
option(PMT_BUILD_TESTS "" ON)

# Find dependencies for PowerSensor
if(${BUILD_POWERSENSOR2_PMT})
  set(POWERSENSOR2_ROOT_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/powersensor2/PowerSensor2)
  set(POWERSENSOR2_SOURCE ${POWERSENSOR2_ROOT_DIR}/PowerSensor.cc)
endif()

if(${BUILD_POWERSENSOR3_PMT})
  set(POWERSENSOR3_ROOT_SRC_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/powersensor3/PowerSensor3/host/src)
  set(POWERSENSOR3_ROOT_INC_DIR
      ${CMAKE_CURRENT_SOURCE_DIR}/powersensor3/PowerSensor3/host/include)
  set(POWERSENSOR3_SOURCE ${POWERSENSOR3_ROOT_SRC_DIR}/PowerSensor.cc
                          ${POWERSENSOR3_ROOT_SRC_DIR}/sensors.cc)
endif()

# Find dependencies for NVML
if(${BUILD_NVML_PMT})
  find_package(CUDAToolkit REQUIRED)
endif()

if(${BUILD_PYTHON_PMT})
  find_package(
    Python3
    COMPONENTS Interpreter Development
    REQUIRED)
  find_package(pybind11 CONFIG REQUIRED)
endif()

# Find dependencies for ROCM
if(${BUILD_ROCM_PMT})
  find_package(ROCM-SMI REQUIRED)
  message(${ROCM-SMI_INCLUDE_DIR})
endif()

# Find dependencies for Likwid
if(${BUILD_LIKWID_PMT})
  find_package(Likwid REQUIRED)
endif()

# Config file
configure_file(${PROJECT_SOURCE_DIR}/cmake/config/pmt.h.in
               ${PROJECT_BINARY_DIR}/pmt.h)

include_directories(${PROJECT_BINARY_DIR})

# Installation
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      ${CMAKE_BINARY_DIR}
      CACHE PATH "pmt install prefix" FORCE)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

include(GNUInstallDirs)

# PMT interface and common functionality
add_subdirectory(common)

# PMT library
add_library(pmt SHARED $<TARGET_OBJECTS:pmt-common>)

# The PMT instances below will append their public header file to this list, so
# that the header file structure can be replicated in the binary directory
# later.
set(PMT_HEADER_FILES "")

# Cray
if(${BUILD_CRAY_PMT})
  add_subdirectory(cray)
endif()

# PowerSensor
if(${BUILD_POWERSENSOR2_PMT})
  add_subdirectory(powersensor2)
endif()
if(${BUILD_POWERSENSOR3_PMT})
  add_subdirectory(powersensor3)
endif()

# Dummy
add_subdirectory(dummy)

# NVML
if(${BUILD_NVML_PMT})
  add_subdirectory(nvml)
endif()

# Tegra
if(${BUILD_TEGRA_PMT})
  add_subdirectory(tegra)
endif()

# Python bindings
if(${pybind11_FOUND})
  add_subdirectory(python)
endif()

# Rapl
if(${BUILD_RAPL_PMT})
  add_subdirectory(rapl)
endif()

# ROCM
if(${ROCM-SMI_FOUND})
  add_subdirectory(rocm)
endif()

# Xilinx
if(${BUILD_XILINX_PMT})
  add_subdirectory(xilinx)
endif()

# Likwid
if(${LIKWID_FOUND})
  add_subdirectory(likwid)
endif()

# Public header file
set_target_properties(
  ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/pmt.h)

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                                   ${CMAKE_CURRENT_BINARY_DIR})
  # Replicate header file structure in binary directory
  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/pmt")
  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/pmt/common")
  file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/common/pmt.h"
       "${PROJECT_BINARY_DIR}/pmt/common/pmt.h" SYMBOLIC)
  foreach(PMT_HEADER_FILE ${PMT_HEADER_FILES})
    set(LINK_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${PMT_HEADER_FILE}")
    get_filename_component(LINK_DST ${PMT_HEADER_FILE} NAME)
    file(CREATE_LINK ${LINK_SRC} "${PROJECT_BINARY_DIR}/pmt/${LINK_DST}"
         SYMBOLIC)
  endforeach()
else()
  # Install library
  target_include_directories(
    pmt PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-config
    COMPONENT ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

  # Install cmake targets
  install(EXPORT ${PROJECT_NAME}-config
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()
