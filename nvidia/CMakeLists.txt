project(pmt-nvidia)

add_library(${PROJECT_NAME} OBJECT NVIDIA.cpp)

set(LINK_LIBRARIES CUDA::nvml Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

if(PMT_BUILD_NVML)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "PMT_BUILD_NVML")
  list(APPEND LINK_LIBRARIES $<TARGET_OBJECTS:pmt-nvml>)
endif()
if(PMT_BUILD_TEGRA)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "PMT_BUILD_TEGRA")
  list(APPEND LINK_LIBRARIES CUDA::cudart $<TARGET_OBJECTS:pmt-tegra>)
endif()

install(FILES NVIDIA.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(NVIDIA-test NVIDIA-test.cpp)
  target_link_libraries(NVIDIA-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(NVIDIA-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS NVIDIA-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "nvidia/NVIDIA.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
