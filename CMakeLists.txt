cmake_minimum_required(VERSION 3.26)
project(psvm)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory("psvm")
add_subdirectory("docs")