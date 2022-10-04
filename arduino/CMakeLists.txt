project(arduino)

add_library(pmt-arduino OBJECT Arduinopmt.cpp ${POWERSENSOR_SOURCE})

target_link_libraries(pmt-arduino OpenMP::OpenMP_C)

install(FILES Arduinopmt.h DESTINATION include/pmt)

add_executable(Arduinopmt-test Arduinopmt-test.cpp)

target_link_libraries(Arduinopmt-test $<TARGET_OBJECTS:pmt-common> pmt-arduino)

install(
  TARGETS Arduinopmt-test
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

target_link_libraries(pmt PUBLIC pmt-arduino)
