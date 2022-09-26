project(dummy)

add_library(
    pmt-dummy
    OBJECT
    Dummypmt.cpp
)

install(
    FILES
    Dummypmt.h
    DESTINATION
    include/pmt
)

target_link_libraries(
    pmt
    PUBLIC
    pmt-dummy
)