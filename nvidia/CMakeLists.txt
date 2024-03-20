project(pmt-nvidia)

include(AddSensor)

set(LINK_LIBRARIES CUDA::nvml CUDA::cudart Threads::Threads)

add_sensor(
  SENSOR_NAME
  "nvidia"
  HEADER
  NVIDIA.h
  SRC_FILES
  NVIDIA.cpp
  LINK_LIBRARIES
  ${LINK_LIBRARIES})

if(PMT_BUILD_NVML)
  find_package(CUDAToolkit REQUIRED)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "PMT_BUILD_NVML")
endif()
if(PMT_BUILD_TEGRA)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "PMT_BUILD_TEGRA")
endif()
