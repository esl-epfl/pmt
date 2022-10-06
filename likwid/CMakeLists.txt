project(likwid)

add_library(pmt-likwid OBJECT Likwid.cpp)

target_link_libraries(pmt-likwid Threads::Threads ${LIKWID_LIBRARY})

target_include_directories(pmt-likwid PRIVATE ${LIKWID_INCLUDE_DIR})

install(FILES Likwid.h DESTINATION include/pmt)

add_executable(Likwid-test Likwid-test.cpp)

target_link_libraries(Likwid-test $<TARGET_OBJECTS:pmt-common> pmt-likwid)

install(TARGETS Likwid-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-likwid)
