project(PMT)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} pmt)

if(${PMT_BUILD_NVIDIA})
  target_link_libraries(${PROJECT_NAME} CUDA::cudart)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION bin)
