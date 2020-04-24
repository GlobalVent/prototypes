
file(GLOB jamctrllib_src . *.cpp *.h)

add_library(jamctrllib STATIC ${jamctrllib_src})
#add_subdirectory(testing)
