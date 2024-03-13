project(pmt-rapl)

add_library(${PROJECT_NAME} OBJECT Rapl.cpp RaplImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(FILES Rapl.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Rapl-test Rapl-test.cpp)

  target_link_libraries(Rapl-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(Rapl-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS Rapl-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "rapl/Rapl.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
