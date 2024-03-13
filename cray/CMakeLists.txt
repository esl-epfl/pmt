project(pmt-cray)

add_library(${PROJECT_NAME} OBJECT Cray.cpp FilenamesHelper.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(FILES Cray.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Cray-test Cray-test.cpp)

  target_link_libraries(Cray-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(Cray-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS Cray-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "cray/Cray.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
