cmake_minimum_required(VERSION 3.17)

project(example)

find_package(CUDAToolkit COMPONENTS nvml)

if(CUDAToolkit_FOUND)
  include(FetchContent)
  set(PMT_BUILD_NVML ON)
  FetchContent_Declare(
    pmt
    GIT_REPOSITORY https://git.astron.nl/RD/pmt
    GIT_TAG ${PMT_GIT_REVISION})
  FetchContent_MakeAvailable(pmt)

  add_executable(example example.cpp)
  target_link_libraries(example pmt)
endif()
