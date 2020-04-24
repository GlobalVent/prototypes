
file(GLOB jamhwlib_src . *.cpp *.h)

add_library(jamhwlib STATIC ${jamhwlib_src})
#add_subdirectory(testing)
