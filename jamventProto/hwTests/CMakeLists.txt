add_executable(HwTests hwTests.cpp)
target_link_libraries(HwTests PRIVATE jamhwlib pigpio)
