project(pmt-nvml)

find_package(CUDAToolkit REQUIRED)

include(AddSensor)
set(LINK_LIBRARIES CUDA::cudart CUDA::nvml Threads::Threads)

add_sensor(
  SENSOR_NAME
  "nvml"
  HEADER
  NVML.h
  SRC_FILES
  NVML.cpp
  NVMLImpl.cpp
  LINK_LIBRARIES
  ${LINK_LIBRARIES})
