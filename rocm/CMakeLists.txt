project(rocm)

add_library(pmt-rocm OBJECT ROCM.cpp)

set(LINK_LIBRARIES Threads::Threads ${ROCM-SMI_LIBRARY})
target_link_libraries(pmt-rocm ${LINK_LIBRARIES})

target_include_directories(pmt-rocm PRIVATE ${ROCM-SMI_INCLUDE_DIR})

install(FILES ROCM.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(ROCM-test ROCM-test.cpp)

  target_link_libraries(ROCM-test $<TARGET_OBJECTS:pmt-common> pmt-rocm
                        ${LINK_LIBRARIES})

  install(TARGETS ROCM-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-rocm)
list(APPEND PMT_HEADER_FILES "rocm/ROCM.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
