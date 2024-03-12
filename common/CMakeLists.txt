project(pmt-common)

add_library(${PROJECT_NAME} OBJECT pmt.cpp pmt-test.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

install(FILES pmt.h DESTINATION include/pmt/common)
