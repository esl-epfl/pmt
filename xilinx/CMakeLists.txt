project(pmt-xilinx)

add_library(${PROJECT_NAME} OBJECT Xilinx.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(FILES Xilinx.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Xilinx-test Xilinx-test.cpp)

  target_link_libraries(Xilinx-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(Xilinx-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS Xilinx-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "xilinx/Xilinx.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
