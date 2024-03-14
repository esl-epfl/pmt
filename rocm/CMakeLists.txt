project(pmt-rocm)

include(AddSensor)

add_library(${PROJECT_NAME} OBJECT ROCM.cpp)

set(LINK_LIBRARIES Threads::Threads ROCM::smi)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

add_sensor(
  SENSOR_NAME
  "rocm"
  HEADER
  ROCM.h
  SRC_FILES
  ROCM.cpp
  LINK_LIBRARIES
  ${LINK_LIBRARIES}
  INCLUDE_DIRECTORIES
  ${INCLUDE_DIRECTORIES}
  TEST
  ROCM-test.cpp)
