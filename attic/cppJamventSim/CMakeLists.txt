cmake_minimum_required(VERSION 2.8.9)
project(cppJamvenSim)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MACOSX_RPATH 1)

include_directories(src)

add_subdirectory(src)
add_subdirectory(testing)

