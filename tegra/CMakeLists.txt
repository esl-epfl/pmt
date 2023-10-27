project(tegra)

add_library(pmt-tegra OBJECT Tegra.cpp TegraImpl.cpp)

set(LINK_LIBRARIES Threads::Threads)
target_link_libraries(pmt-tegra ${LINK_LIBRARIES})

install(FILES Tegra.h DESTINATION include/pmt)

add_executable(Tegra-test Tegra-test.cpp)

target_link_libraries(Tegra-test $<TARGET_OBJECTS:pmt-common> pmt-tegra
                      ${LINK_LIBRARIES})

install(TARGETS Tegra-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PRIVATE pmt-tegra)
