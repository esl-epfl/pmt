project(pmt-powersensor2)

add_library(${PROJECT_NAME} OBJECT PowerSensor2.cpp ${POWERSENSOR2_SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}
                                                   ${POWERSENSOR2_ROOT_DIR})

set(LINK_LIBRARIES OpenMP::OpenMP_C)
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

install(FILES PowerSensor2.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(PowerSensor2-test PowerSensor2-test.cpp)

  target_link_libraries(PowerSensor2-test $<TARGET_OBJECTS:pmt-common>
                        $<TARGET_OBJECTS:${PROJECT_NAME}> ${LINK_LIBRARIES})
  target_include_directories(PowerSensor2-test PRIVATE ${CMAKE_SOURCE_DIR})

  install(
    TARGETS PowerSensor2-test
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE ${PROJECT_NAME})
list(APPEND PMT_HEADER_FILES "powersensor2/PowerSensor2.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
