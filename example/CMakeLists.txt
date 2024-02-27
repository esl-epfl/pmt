project(example)

cmake_minimum_required(VERSION 3.17)

find_package(CUDAToolkit COMPONENTS nvml)

if(CUDAToolkit_FOUND)
  include(FetchContent)
  set(BUILD_NVML_PMT ON)
  set(PMT_BUILD_TESTS OFF)
  FetchContent_Declare(pmt GIT_REPOSITORY https://git.astron.nl/RD/pmt)
  FetchContent_MakeAvailable(pmt)

  add_executable(example example.cpp)
  target_link_libraries(example pmt)
endif()
