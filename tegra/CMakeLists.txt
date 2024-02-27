project(tegra)

add_library(pmt-tegra OBJECT Tegra.cpp TegraImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-tegra ${LINK_LIBRARIES})

install(FILES Tegra.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(Tegra-test Tegra-test.cpp)

  target_link_libraries(Tegra-test $<TARGET_OBJECTS:pmt-common> pmt-tegra
                        ${LINK_LIBRARIES})

  install(TARGETS Tegra-test RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-tegra)
list(APPEND PMT_HEADER_FILES "tegra/Tegra.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
