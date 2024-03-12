project(pmt-tegra)

add_library(${PROJECT_NAME} OBJECT Tegra.cpp TegraImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

install(FILES Tegra.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Tegra-test Tegra-test.cpp)

  target_link_libraries(Tegra-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(Tegra-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(TARGETS Tegra-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "tegra/Tegra.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
