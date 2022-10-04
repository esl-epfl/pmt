project(jetson)

add_library(pmt-jetson OBJECT Jetsonpmt.cpp)

target_link_libraries(pmt-jetson Threads::Threads)

install(FILES Jetsonpmt.h DESTINATION include/pmt)

add_executable(Jetsonpmt-test Jetsonpmt-test.cpp)

target_link_libraries(Jetsonpmt-test $<TARGET_OBJECTS:pmt-common> pmt-jetson)

install(TARGETS Jetsonpmt-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-jetson)
