cmake_minimum_required(VERSION 3.26)
project(psvm)

set(CMAKE_CXX_STANDARD 17)

add_executable(psvm src/test_driver.cpp src/psvm.cpp)

target_include_directories(psvm PRIVATE include lib/quickjs)
target_link_libraries(psvm ${CMAKE_SOURCE_DIR}/lib/quickjs/libquickjs.a)