cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB jamventsim_src . *.cpp *.h)

add_library(jamventsim STATIC ${jamventsim_src})
add_subdirectory(testing)


# see if we can move this into the testing subdirectory
enable_testing()
add_test(testConf testing/testConf)
add_test(testRes testing/testRes)
add_test(testLungsComp0 testing/testLungsComp0)
add_test(testLungsCompInf testing/testLungsCompInf)
# todo, correct the math so these two tests will pass
add_test(testLungsComp1 testing/testLungsComp1)
add_test(testLungsComp2 testing/testLungsComp2)

