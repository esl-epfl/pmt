project(pmt)

cmake_minimum_required(VERSION 3.17.5)

# Set cmake module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Set build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Default build type." FORCE)
endif()

# Set debug print info
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# Enable OpenMP
find_package(OpenMP REQUIRED)

# Enable PThreads
find_package(Threads REQUIRED)

# Add PMT common directory as include directory
include_directories(${CMAKE_SOURCE_DIR}/common)

# Options to disable implementations
# that require external dependencies
option(BUILD_ARDUINO_PMT "" OFF)
option(BUILD_LIKWID_PMT "" OFF)
option(BUILD_NVML_PMT "" OFF)
option(BUILD_PYTHON_PMT "" OFF)
option(BUILD_ROCM_PMT "" OFF)

# Find dependencies for Arduino
if(${BUILD_ARDUINO_PMT})
  set(POWERSENSOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/arduino/PowerSensor)
  set(POWERSENSOR_HEADER ${POWERSENSOR_ROOT_DIR}/PowerSensor.h)
  set(POWERSENSOR_SOURCE ${POWERSENSOR_ROOT_DIR}/PowerSensor.cc)
endif()

# Find dependencies for Likwid
if(${BUILD_LIKWID_PMT})
  find_package(Likwid QUIET)
  if(${LIKWID_FOUND})
    set(HAVE_LIKWID TRUE)
  else()
    message("Likwid was not found, Likwid will not be built.")
  endif()
endif()

# Find dependencies for NVML
if(${BUILD_NVML_PMT})
  find_package(CUDAToolkit QUIET)
  if(${CUDAToolkit_FOUND})
    set(HAVE_NVML TRUE)
  endif()
endif()

if(${BUILD_PYTHON_PMT})
  find_package(
    Python3
    COMPONENTS Interpreter Development
    REQUIRED)
  find_package(pybind11 CONFIG REQUIRED)
endif()

# Find dependencies for ROCMpmt
if(${BUILD_ROCM_PMT})
  find_package(ROCM-SMI)
  if(${ROCM-SMI_FOUND})
    set(HAVE_ROCMSMI TRUE)
  else()
    message("ROCM-SMI was not found, ROCMpmt will not be built.")
  endif()
endif()

# Config file
configure_file(${PROJECT_SOURCE_DIR}/cmake/config/pmt-config.h.in
               ${PROJECT_BINARY_DIR}/pmt-config.h)

include_directories(${PROJECT_BINARY_DIR})
install(FILES ${PROJECT_BINARY_DIR}/pmt-config.h DESTINATION include)

# Installation
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      ${CMAKE_BINARY_DIR}
      CACHE PATH "pmt install prefix" FORCE)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH True)

# PMT interface and common functionality
add_subdirectory(common)

# PMT library
add_library(pmt SHARED $<TARGET_OBJECTS:pmt-common>)

# AMDGPU
add_subdirectory(amdgpu)

# Arduino
if(${BUILD_ARDUINO_PMT})
  add_subdirectory(arduino)
endif()

# Dummy
add_subdirectory(dummy)

# Likwid
if(${LIKWID_FOUND})
  add_subdirectory(likwid)
endif()

# NVML
if(${CUDAToolkit_FOUND})
  add_subdirectory(nvml)
endif()

# Jetson
add_subdirectory(jetson)

#Python Bindings
if(${pybind11_FOUND})
  add_subdirectory(python)
endif()

# Raplpmt
add_subdirectory(rapl)

# ROCMpmt
if(${ROCM-SMI_FOUND})
  add_subdirectory(rocm)
endif()

# Xilinxpmt
add_subdirectory(xilinx)

# Public header file
set_target_properties(pmt PROPERTIES PUBLIC_HEADER pmt.h)

# Install library
install(
  TARGETS pmt
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include)
