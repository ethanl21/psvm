cmake_minimum_required(VERSION 3.26)

add_subdirectory("lib")

add_library(psvm STATIC src/psvm.cpp)

target_include_directories(psvm PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/quickjspp")
target_link_libraries(psvm PRIVATE quickjs)


get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

add_custom_target(generate_psvmjs
        COMMAND qjsc "-c" "-o" "${CMAKE_CURRENT_SOURCE_DIR}/src/psvmjs.c" "${PARENT_DIR}/psvmjs/dist/globalize.js")

add_dependencies(generate_psvmjs qjsc)

add_dependencies(psvm generate_psvmjs)