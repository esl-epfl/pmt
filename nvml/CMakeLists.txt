project(pmt-nvml)

include(AddSensor)
set(LINK_LIBRARIES CUDA::nvml Threads::Threads)

add_sensor(
  SENSOR_NAME
  "nvml"
  HEADER
  NVML.h
  SRC_FILES
  NVML.cpp
  LINK_LIBRARIES
  ${LINK_LIBRARIES}
  TEST
  NVML-test.cpp)
