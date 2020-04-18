file(GLOB testing_src . *.cpp *.h)

include_directories(..)
add_executable(jsimTestConf testConf.cpp)
target_link_libraries(jsimTestConf "${LIBRARIES_FROM_REFERENCES}")

