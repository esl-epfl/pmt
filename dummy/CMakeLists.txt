project(dummy)

add_library(pmt-dummy OBJECT Dummy.cpp)

install(FILES Dummy.h DESTINATION include/pmt)

target_link_libraries(pmt PRIVATE pmt-dummy)
list(APPEND PMT_HEADER_FILES "dummy/Dummy.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
