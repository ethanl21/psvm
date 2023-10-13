cmake_minimum_required(VERSION 3.26)

add_executable(psvm_test test_driver.cpp)
target_link_libraries(psvm_test PRIVATE psvm)