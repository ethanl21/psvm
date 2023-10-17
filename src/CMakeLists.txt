add_custom_target(generate_psvmjs_c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs
        COMMENT "Generating psvmjs.c..."
        COMMAND ${NPM_EXECUTABLE} "i"
        COMMAND ${NPM_EXECUTABLE} "run" "build"
        COMMAND qjsc "-c" "-o" "${CMAKE_CURRENT_SOURCE_DIR}/psvmjs.c" "${CMAKE_CURRENT_SOURCE_DIR}/psvmjs/dist/globalize.js"
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs.c ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs/dist/globalize.js
)

add_dependencies(generate_psvmjs_c qjsc)

add_dependencies(psvm generate_psvmjs_c)

target_sources(psvm
        PRIVATE
        psvm.cpp
        psvmjs.c
        "${CMAKE_SOURCE_DIR}/include/psvm/psvm.hpp"
)

target_include_directories(psvm
        PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/extern/quickjspp"
)