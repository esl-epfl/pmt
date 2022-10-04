project(nvml)

add_library(pmt-nvml OBJECT NVMLpmt.cpp)

target_link_libraries(pmt-nvml CUDA::nvml Threads::Threads)

install(FILES NVMLpmt.h DESTINATION include/pmt)

add_executable(NVMLpmt-test NVMLpmt-test.cpp)

target_link_libraries(NVMLpmt-test $<TARGET_OBJECTS:pmt-common> pmt-nvml)

install(TARGETS NVMLpmt-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-nvml)
