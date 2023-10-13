cmake_minimum_required(VERSION 3.26)
project(psvm)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(psvm src/test_driver.cpp src/psvm.cpp
        include/psvmjs.hpp
        include/psvmjs.c)

target_include_directories(psvm PRIVATE include lib/quickjs)

target_link_libraries(psvm PRIVATE Boost::headers ${CMAKE_SOURCE_DIR}/lib/quickjs/libquickjs.lto.a)