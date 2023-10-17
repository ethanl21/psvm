cmake_minimum_required(VERSION 3.26)
find_package(Boost REQUIRED)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)
endif()

add_library(psvm STATIC psvm.cpp)

target_link_libraries(psvm PRIVATE Boost::headers quickjs)
target_include_directories(psvm PRIVATE "${CMAKE_SOURCE_DIR}/psvm/lib/quickjspp")

add_dependencies(psvm generate_psvmjs)
