set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)


include_directories(../utils)
include_directories(../jamctrllib)


add_executable(testCtrl01 testCtrl01.cpp)
target_link_libraries(testCtrl01 jamctrllib jamventsim pthread "${LIBRARIES_FROM_REFERENCES}")
