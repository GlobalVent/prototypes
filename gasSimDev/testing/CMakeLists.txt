file(GLOB testing_src . *.cpp *.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include_directories(..)

include_directories(../utils)
set(JV_SIM_TESTLIBS jamventsim)

add_executable(testConf testConf.cpp)
target_link_libraries(testConf "${LIBRARIES_FROM_REFERENCES}")

add_executable(testRes testRes.cpp)
target_link_libraries(testRes ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsComp0 testLungsComp0.cpp)
target_link_libraries(testLungsComp0 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testLungsCompInf testLungsCompInf.cpp)
target_link_libraries(testLungsCompInf ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testModel00 testModel00.cpp)
target_link_libraries(testModel00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")
