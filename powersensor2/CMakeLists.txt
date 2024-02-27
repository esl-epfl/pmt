project(powersensor2)

add_library(pmt-powersensor2 OBJECT PowerSensor2.cpp ${POWERSENSOR2_SOURCE})

target_include_directories(pmt-powersensor2 PRIVATE ${POWERSENSOR2_ROOT_DIR})

set(LINK_LIBRARIES OpenMP::OpenMP_C)
target_link_libraries(pmt-powersensor2 ${LINK_LIBRARIES})

install(FILES PowerSensor2.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(PowerSensor2-test PowerSensor2-test.cpp)

  target_link_libraries(PowerSensor2-test $<TARGET_OBJECTS:pmt-common>
                        pmt-powersensor2 ${LINK_LIBRARIES})

  install(
    TARGETS PowerSensor2-test
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-powersensor2)
list(APPEND PMT_HEADER_FILES "powersensor2/PowerSensor2.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
