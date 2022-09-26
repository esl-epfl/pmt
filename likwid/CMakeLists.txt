project(likwid)

add_library(
    pmt-likwid
    OBJECT
    Likwidpmt.cpp
)

target_link_libraries(
    pmt-likwid
    Threads::Threads
    ${LIKWID_LIBRARY}
)

target_include_directories(
    pmt-likwid
    PRIVATE
    ${LIKWID_INCLUDE_DIR}
)

install(
    FILES
    Likwidpmt.h
    DESTINATION
    include/pmt
)

add_executable(
    Likwidpmt-test
    Likwidpmt-test.cpp
)

target_link_libraries(
    Likwidpmt-test
    $<TARGET_OBJECTS:pmt-common>
    pmt-likwid
)

install(
    TARGETS
    Likwidpmt-test
    RUNTIME DESTINATION bin
)

target_link_libraries(
    pmt
    PUBLIC
    pmt-likwid
)
