project(xilinx)

add_library(pmt-xilinx OBJECT Xilinx.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-xilinx ${LINK_LIBRARIES})

install(FILES Xilinx.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Xilinx-test Xilinx-test.cpp)

  target_link_libraries(Xilinx-test $<TARGET_OBJECTS:pmt-common> pmt-xilinx
                        ${LINK_LIBRARIES})

  install(TARGETS Xilinx-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-xilinx)
list(APPEND PMT_HEADER_FILES "xilinx/Xilinx.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
