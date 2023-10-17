cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

add_library(psvm STATIC psvm.cpp)

target_include_directories(psvm PRIVATE "${CMAKE_SOURCE_DIR}/psvm/lib/quickjspp")
target_link_libraries(psvm PRIVATE quickjs)

add_dependencies(psvm generate_psvmjs)
