cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB jamventsim_src . *.cpp *.h)

add_library(jamventsim STATIC ${jamventsim_src})
add_subdirectory(testing)
