
file(GLOB jamhwlib_src . *.cpp *.h)

add_library(jamhwlib STATIC ${jamhwlib_src} I2cGenericDev.cpp I2cMSxxx.cpp I2cMS5607.cpp I2cMS5803.cpp I2cADC128D818.cpp O2Sensor.cpp I2cADC128D818.h O2Sensor.h I2cAdcWithO2.h)
#add_subdirectory(testing)
