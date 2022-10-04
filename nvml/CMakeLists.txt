project(nvml)

add_library(pmt-nvml OBJECT NVML.cpp)

target_link_libraries(pmt-nvml CUDA::nvml Threads::Threads)

install(FILES NVML.h DESTINATION include/pmt)

add_executable(NVML-test NVML-test.cpp)

target_link_libraries(NVML-test $<TARGET_OBJECTS:pmt-common> pmt-nvml)

install(TARGETS NVML-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-nvml)
