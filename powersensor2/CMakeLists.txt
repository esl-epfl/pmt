project(pmt-powersensor2)

include(AddSensor)
set(LINK_LIBRARIES OpenMP::OpenMP_C)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR} ${POWERSENSOR2_ROOT_DIR})

add_sensor(
  SENSOR_NAME
  "powersensor2"
  HEADER
  PowerSensor2.h
  SRC_FILES
  PowerSensor2.cpp
  ${POWERSENSOR2_SOURCE}
  LINK_LIBRARIES
  ${LINK_LIBRARIES}
  INCLUDE_DIRECTORIES
  ${INCLUDE_DIRECTORIES}
  TEST
  PowerSensor2-test.cpp)
