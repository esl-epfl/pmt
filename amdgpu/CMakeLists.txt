project(amdgpu)

add_library(pmt-amdgpu OBJECT AMDGPU.cpp)

target_link_libraries(pmt-amdgpu Threads::Threads)

install(FILES AMDGPU.h DESTINATION include/pmt)

add_executable(AMDGPU-test AMDGPU-test.cpp)

target_link_libraries(AMDGPU-test $<TARGET_OBJECTS:pmt-common> pmt-amdgpu)

install(TARGETS AMDGPU-test RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-amdgpu)
