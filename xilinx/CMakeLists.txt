project(xilinx)

add_library(pmt-xilinx OBJECT Xilinxpmt.cpp)

target_link_libraries(pmt-xilinx Threads::Threads)

install(FILES Xilinxpmt.h DESTINATION include/pmt)

add_executable(Xilinxpmt-test Xilinxpmt-test.cpp)

target_link_libraries(Xilinxpmt-test $<TARGET_OBJECTS:pmt-common> pmt-xilinx)

install(TARGETS Xilinxpmt-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-xilinx)
