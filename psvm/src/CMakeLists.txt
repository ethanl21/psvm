cmake_minimum_required(VERSION 3.26)
find_package(Boost REQUIRED)

add_library(psvm STATIC psvm.cpp)
target_link_libraries(psvm PRIVATE Boost::headers quickjs)
target_include_directories(psvm PUBLIC "${CMAKE_SOURCE_DIR}/psvm/lib/quickjspp")
