project(likwid)

add_library(pmt-likwid OBJECT Likwid.cpp LikwidImpl.cpp)

target_link_libraries(pmt-likwid Threads::Threads ${LIKWID_LIBRARY})

target_include_directories(pmt-likwid PRIVATE ${LIKWID_INCLUDE_DIR})

install(FILES Likwid.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Likwid-test Likwid-test.cpp)

  target_link_libraries(Likwid-test $<TARGET_OBJECTS:pmt-common> pmt-likwid
                        ${LIKWID_LIBRARY})

  install(TARGETS Likwid-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-likwid)
list(APPEND PMT_HEADER_FILES "likwid/Likwid.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
