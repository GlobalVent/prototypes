
file(GLOB jamhwlib_src . *.cpp *.h)

add_library(jamhwlib STATIC ${jamhwlib_src} I2cGenericDev.cpp I2cMSxxx.cpp I2cMS5607.cpp I2cMS5803.cpp I2cADC128D818.cpp I2cADC128D818.h I2cAdcWithO2.cpp Valves.cpp Speaker.cpp Fpga.cpp JamHwErrors.cpp)
#add_subdirectory(testing)
