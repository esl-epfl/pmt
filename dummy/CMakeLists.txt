project(dummy)

add_library(pmt-dummy OBJECT Dummy.cpp)

install(FILES Dummy.h DESTINATION include/pmt)

target_link_libraries(pmt PRIVATE pmt-dummy)
