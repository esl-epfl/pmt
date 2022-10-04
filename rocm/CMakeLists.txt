project(rocm)

add_library(pmt-rocm OBJECT ROCMpmt.cpp)

target_link_libraries(pmt-rocm Threads::Threads ${ROCM-SMI_LIBRARY})

target_include_directories(pmt-rocm PRIVATE ${ROCM-SMI_INCLUDE_DIR})

install(FILES ROCMpmt.h DESTINATION include/pmt)

add_executable(ROCMpmt-test ROCMpmt-test.cpp)

target_link_libraries(ROCMpmt-test $<TARGET_OBJECTS:pmt-common> pmt-rocm)

install(TARGETS ROCMpmt-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-rocm)
