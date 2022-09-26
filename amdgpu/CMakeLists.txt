project(amdgpu)

add_library(
    pmt-amdgpu
    OBJECT
    AMDGPUpmt.cpp
)

target_link_libraries(
    pmt-amdgpu
    Threads::Threads
)

install(
    FILES
    AMDGPUpmt.h
    DESTINATION
    include/pmt
)

add_executable(
    AMDGPUpmt-test
    AMDGPUpmt-test.cpp
)

target_link_libraries(
    AMDGPUpmt-test
    $<TARGET_OBJECTS:pmt-common>
    pmt-amdgpu
)

install(
    TARGETS
    AMDGPUpmt-test
    RUNTIME DESTINATION bin
)

target_link_libraries(
    pmt
    PUBLIC
    pmt-amdgpu
)
