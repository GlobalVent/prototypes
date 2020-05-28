
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB jamctrllib_src . *.cpp *.h)

add_library(jamctrllib STATIC ${jamctrllib_src})
#add_subdirectory(testing)
