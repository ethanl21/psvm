cmake_minimum_required(VERSION 3.26)

add_executable(psvm_test test_driver.cpp)
target_include_directories(psvm_test PRIVATE "${CMAKE_SOURCE_DIR}/psvm/src")
target_link_libraries(psvm_test PRIVATE psvm)