project(powersensor3)

add_library(pmt-powersensor3 OBJECT PowerSensor3.cpp ${POWERSENSOR3_SOURCE})
target_include_directories(pmt-powersensor3
                           PRIVATE ${POWERSENSOR3_ROOT_INC_DIR})
target_link_libraries(pmt-powersensor3 OpenMP::OpenMP_C)

install(FILES PowerSensor3.h DESTINATION include/pmt)

add_executable(PowerSensor3-test PowerSensor3-test.cpp)

target_link_libraries(PowerSensor3-test $<TARGET_OBJECTS:pmt-common>
                      pmt-powersensor3)

install(
  TARGETS PowerSensor3-test
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-powersensor3)
