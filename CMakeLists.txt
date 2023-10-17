cmake_minimum_required(VERSION 3.26)

## hack to fix collision between the VERSION file and the c++ std <version> header
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/VERSION")
    file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/VERSION" "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/_VERSION")
endif ()

# Add paths to modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Main project info
project(psvm
        VERSION 0.1)

# Compile universal binaries for macOS
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
endif ()

# PSVM's static library
add_library(psvm STATIC)

# Use C++ 17
target_compile_features(psvm
        PRIVATE
        cxx_std_17)

# Compile universal binaries for macOS
set(LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR})
if (APPLE)
    set(LIB_ARCH "universal")
endif ()

# Find Node.js and npm to bundle psvmjs
include(NodeJS)

# Add src directory
add_subdirectory(src)

# Turn on ccache if found
include(ccache)

# Format all the code if ClangFormat is found
include(ClangFormat)

add_subdirectory(extern)

target_link_libraries(psvm
        PRIVATE
        quickjs)

# Add test directory
add_subdirectory(test)

target_link_libraries(psvm_test
        PRIVATE
        psvm
)

## hack to fix collision between the VERSION file and the c++ std <version> header
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/_VERSION")
    file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/_VERSION" "${CMAKE_CURRENT_SOURCE_DIR}/extern/quickjspp/VERSION")
endif ()