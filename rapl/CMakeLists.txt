project(rapl)

add_library(pmt-rapl OBJECT Raplpmt.cpp rapl-read.cpp)

target_link_libraries(pmt-rapl Threads::Threads)

install(FILES Raplpmt.h DESTINATION include/pmt)

add_executable(Raplpmt-test Raplpmt-test.cpp)

target_link_libraries(Raplpmt-test $<TARGET_OBJECTS:pmt-common> pmt-rapl)

install(TARGETS Raplpmt-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-rapl)
