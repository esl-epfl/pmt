project(xilinx)

add_library(pmt-xilinx OBJECT Xilinx.cpp)

target_link_libraries(pmt-xilinx Threads::Threads)

install(FILES Xilinx.h DESTINATION include/pmt)

add_executable(Xilinx-test Xilinx-test.cpp)

target_link_libraries(Xilinx-test $<TARGET_OBJECTS:pmt-common> pmt-xilinx)

install(TARGETS Xilinx-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-xilinx)
