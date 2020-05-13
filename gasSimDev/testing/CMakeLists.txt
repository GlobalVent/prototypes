file(GLOB testing_src . *.cpp *.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include_directories(..)

include_directories(../utils)
set(JV_SIM_TESTLIBS jamventsim)

#foreach(exe testConf testRes testLungsComp0 testLungsComp1 testLungsCompInf testModel00)
#    add_executable(${exe} ${exe}.cpp)
#    target_link_libraries(${exe} "${LIBRARIES_FROM_REFERENCES}")
#endforeach(exe);

add_executable(testConf testConf.cpp)
target_link_libraries(testConf "${LIBRARIES_FROM_REFERENCES}")

add_executable(testRes testRes.cpp)
target_link_libraries(testRes ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsComp0 testLungsComp0.cpp)
target_link_libraries(testLungsComp0 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsComp1 testLungsComp1.cpp)
target_link_libraries(testLungsComp1 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsComp2 testLungsComp2.cpp)
target_link_libraries(testLungsComp2 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsCompInf testLungsCompInf.cpp)
target_link_libraries(testLungsCompInf ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testModel00 testModel00.cpp)
target_link_libraries(testModel00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testResPo2 testResPo2.cpp)
target_link_libraries(testResPo2 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")


