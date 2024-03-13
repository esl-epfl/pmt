project(pmt-dummy)

add_library(${PROJECT_NAME} OBJECT Dummy.cpp)
target_link_libraries(pmt PRIVATE ${PROJECT_NAME})

install(FILES Dummy.h DESTINATION include/pmt)

list(APPEND PMT_HEADER_FILES "dummy/Dummy.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
