add_executable(I2cTests I2cTests.cpp)
target_link_libraries(I2cTests PRIVATE jamhwlib pigpio)
