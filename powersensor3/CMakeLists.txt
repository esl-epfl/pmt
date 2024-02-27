project(powersensor3)

add_library(pmt-powersensor3 OBJECT PowerSensor3.cpp ${POWERSENSOR3_SOURCE})

target_include_directories(pmt-powersensor3
                           PRIVATE ${POWERSENSOR3_ROOT_INC_DIR})

set(LINK_LIBRARIES OpenMP::OpenMP_C)
target_link_libraries(pmt-powersensor3 ${LINK_LIBRARIES})

install(FILES PowerSensor3.h DESTINATION include/pmt)

if(PMT_BUILD_TESTS)
  add_executable(PowerSensor3-test PowerSensor3-test.cpp)

  target_link_libraries(PowerSensor3-test $<TARGET_OBJECTS:pmt-common>
                        pmt-powersensor3 ${LINK_LIBRARIES})

  install(
    TARGETS PowerSensor3-test
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
endif()

target_link_libraries(pmt PRIVATE pmt-powersensor3)
list(APPEND PMT_HEADER_FILES "powersensor3/PowerSensor3.h")
set(PMT_HEADER_FILES
    ${PMT_HEADER_FILES}
    PARENT_SCOPE)
