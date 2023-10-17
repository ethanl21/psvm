cmake_minimum_required(VERSION 3.26)

add_executable(psvm_test test_driver.cpp)

# Use C++ 17
target_compile_features(psvm_test
        PRIVATE
        cxx_std_17)

# Set include path
target_include_directories(psvm_test
        PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
)