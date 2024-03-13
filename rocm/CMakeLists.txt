project(pmt-rocm)

add_library(${PROJECT_NAME} OBJECT ROCM.cpp)

set(LINK_LIBRARIES Threads::Threads ROCM::smi)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(FILES ROCM.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(ROCM-test ROCM-test.cpp)

  target_link_libraries(ROCM-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(ROCM-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS ROCM-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "rocm/ROCM.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
