project(jetson)

add_library(pmt-jetson OBJECT Jetson.cpp)

target_link_libraries(pmt-jetson Threads::Threads)

install(FILES Jetson.h DESTINATION include/pmt)

add_executable(Jetson-test Jetson-test.cpp)

target_link_libraries(Jetson-test $<TARGET_OBJECTS:pmt-common> pmt-jetson)

install(TARGETS Jetson-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-jetson)
