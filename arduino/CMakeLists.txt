project(arduino)

add_library(pmt-arduino OBJECT Arduino.cpp ${POWERSENSOR_SOURCE}
                               ${POWERSENSOR3_SOURCE})
target_include_directories(pmt-arduino PRIVATE ${POWERSENSOR_ROOT_INC_DIR}
                                               ${POWERSENSOR3_ROOT_INC_DIR})
target_link_libraries(pmt-arduino OpenMP::OpenMP_C)

install(FILES Arduino.h DESTINATION include/pmt)

add_executable(Arduino-test Arduino-test.cpp)
add_executable(Arduino-test-v3 Arduino-test-v3.cpp)

target_link_libraries(Arduino-test $<TARGET_OBJECTS:pmt-common> pmt-arduino)
target_link_libraries(Arduino-test-v3 $<TARGET_OBJECTS:pmt-common> pmt-arduino)

install(
  TARGETS Arduino-test Arduino-test-v3
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-arduino)
