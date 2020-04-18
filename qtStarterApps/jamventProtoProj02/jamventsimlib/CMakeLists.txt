
file(GLOB jamventsim_src . *.cpp *.h)

add_library(jamventsim STATIC ${jamventsim_src})
add_subdirectory(testing)
