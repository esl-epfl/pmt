project(cray)

add_library(pmt-cray OBJECT Cray.cpp FilenamesHelper.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-cray ${LINK_LIBRARIES})

target_include_directories(pmt-cray PRIVATE)

install(FILES Cray.h DESTINATION include/pmt)

add_executable(Cray-test Cray-test.cpp)

target_link_libraries(Cray-test $<TARGET_OBJECTS:pmt-common> pmt-cray
                      ${LINK_LIBRARIES})

install(TARGETS Cray-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PRIVATE pmt-cray)
