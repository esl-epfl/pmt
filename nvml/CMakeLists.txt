project(nvml)

add_library(pmt-nvml OBJECT NVML.cpp)

set(LINK_LIBRARIES CUDA::nvml Threads::Threads)
target_link_libraries(pmt-nvml ${LINK_LIBRARIES})

install(FILES NVML.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(NVML-test NVML-test.cpp)

  target_link_libraries(NVML-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:pmt-nvml> ${LINK_LIBRARIES})

  install(TARGETS NVML-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-nvml)
list(APPEND PMT_HEADER_FILES "nvml/NVML.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
