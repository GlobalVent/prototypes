file(GLOB testing_src . *.cpp *.h)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include_directories(..)

include_directories(../utils)
set(JV_SIM_TESTLIBS jamventsim)


add_executable(testConf testConf.cpp)
target_link_libraries(testConf "${LIBRARIES_FROM_REFERENCES}")

add_executable(test00 test00.cpp)
target_link_libraries(test00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(test01 test01.cpp)
target_link_libraries(test01 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(testModel00 testModel00.cpp)
target_link_libraries(testModel00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

