project(xilinx)

add_library(pmt-xilinx OBJECT Xilinx.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-xilinx ${LINK_LIBRARIES})

install(FILES Xilinx.h DESTINATION include/pmt)

add_executable(Xilinx-test Xilinx-test.cpp)

target_link_libraries(Xilinx-test $<TARGET_OBJECTS:pmt-common> pmt-xilinx
                      ${LINK_LIBRARIES})

install(TARGETS Xilinx-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PRIVATE pmt-xilinx)
