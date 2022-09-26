project(pmt-common)

add_library(
    ${PROJECT_NAME}
    SHARED
    pmt.cpp
    pmt-test.cpp
)

install(
    FILES
    pmt.h
    DESTINATION
    include/pmt
)