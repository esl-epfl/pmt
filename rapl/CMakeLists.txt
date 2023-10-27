project(rapl)

add_library(pmt-rapl OBJECT Rapl.cpp RaplImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-rapl ${LINK_LIBRARIES})

install(FILES Rapl.h DESTINATION include/pmt)

add_executable(Rapl-test Rapl-test.cpp)

target_link_libraries(Rapl-test $<TARGET_OBJECTS:pmt-common> pmt-rapl
                      ${LINK_LIBRARIES})

install(TARGETS Rapl-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PRIVATE pmt-rapl)
