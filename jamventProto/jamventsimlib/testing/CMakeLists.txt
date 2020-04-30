file(GLOB testing_src . *.cpp *.h)

include_directories(..)
set(JV_SIM_TESTLIBS jamventsim)
add_executable(jsimTestConf testConf.cpp)
target_link_libraries(jsimTestConf "${LIBRARIES_FROM_REFERENCES}")

add_executable(jsimTest00 test00.cpp)
target_link_libraries(jsimTest00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

add_executable(jsimModel00 testModel00.cpp)
target_link_libraries(jsimModel00 ${JV_SIM_TESTLIBS} "${LIBRARIES_FROM_REFERENCES}")

