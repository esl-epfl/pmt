project(rapl)

add_library(pmt-rapl OBJECT Rapl.cpp rapl-read.cpp)

target_link_libraries(pmt-rapl Threads::Threads)

install(FILES Rapl.h DESTINATION include/pmt)

add_executable(Rapl-test Rapl-test.cpp)

target_link_libraries(Rapl-test $<TARGET_OBJECTS:pmt-common> pmt-rapl)

install(TARGETS Rapl-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-rapl)
