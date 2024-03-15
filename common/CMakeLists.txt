project(pmt-common)

add_library(${PROJECT_NAME} OBJECT pmt.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PMT_INCLUDE_DIR})

install(FILES pmt.h DESTINATION include/pmt/common)
