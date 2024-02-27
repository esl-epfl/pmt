project(rapl)

add_library(pmt-rapl OBJECT Rapl.cpp RaplImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-rapl ${LINK_LIBRARIES})

install(FILES Rapl.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Rapl-test Rapl-test.cpp)

  target_link_libraries(Rapl-test $<TARGET_OBJECTS:pmt-common> pmt-rapl
                        ${LINK_LIBRARIES})

  install(TARGETS Rapl-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-rapl)
list(APPEND PMT_HEADER_FILES "rapl/Rapl.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
