project(rocm)

add_library(pmt-rocm OBJECT ROCM.cpp)

target_link_libraries(pmt-rocm Threads::Threads ${ROCM-SMI_LIBRARY})

target_include_directories(pmt-rocm PRIVATE ${ROCM-SMI_INCLUDE_DIR})

install(FILES ROCM.h DESTINATION include/pmt)

add_executable(ROCM-test ROCM-test.cpp)

target_link_libraries(ROCM-test $<TARGET_OBJECTS:pmt-common> pmt-rocm)

install(TARGETS ROCM-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-rocm)
