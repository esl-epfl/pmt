project(arduino)

add_library(pmt-arduino OBJECT Arduino.cpp ${POWERSENSOR_SOURCE})

target_link_libraries(pmt-arduino OpenMP::OpenMP_C)

install(FILES Arduino.h DESTINATION include/pmt)

add_executable(Arduino-test Arduino-test.cpp)

target_link_libraries(Arduino-test $<TARGET_OBJECTS:pmt-common> pmt-arduino)

install(
  TARGETS Arduino-test
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-arduino)
