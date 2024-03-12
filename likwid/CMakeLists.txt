project(pmt-likwid)

add_library(${PROJECT_NAME} OBJECT Likwid.cpp LikwidImpl.cpp)
target_link_libraries(${PROJECT_NAME} Threads::Threads ${LIKWID_LIBRARY})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIKWID_INCLUDE_DIR})

install(FILES Likwid.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Likwid-test Likwid-test.cpp)

  target_link_libraries(Likwid-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LIKWID_LIBRARY})
  target_include_directories(Likwid-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS Likwid-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "likwid/Likwid.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
