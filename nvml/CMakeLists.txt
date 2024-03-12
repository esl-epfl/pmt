project(pmt-nvml)

add_library(${PROJECT_NAME} OBJECT NVML.cpp)

set(LINK_LIBRARIES CUDA::nvml Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

install(FILES NVML.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(NVML-test NVML-test.cpp)

  target_link_libraries(NVML-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(NVML-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS NVML-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "nvml/NVML.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
