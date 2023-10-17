# Bundles the JavaScript module and compiles to bytecode
add_custom_target(generate_psvmjs_c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs
        COMMENT "Generating psvmjs.c..."
        COMMAND ${NPM_EXECUTABLE} "i"
        COMMAND ${NPM_EXECUTABLE} "run" "build"
        COMMAND qjsc "-c" "-o" "${CMAKE_CURRENT_SOURCE_DIR}/psvmjs.c" "${CMAKE_CURRENT_SOURCE_DIR}/psvmjs/dist/globalize.js"
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs.c ${CMAKE_CURRENT_SOURCE_DIR}/psvmjs/dist/globalize.js
)

# qjsc is required to compile js to bytecode
add_dependencies(generate_psvmjs_c qjsc)

# psvmjs' bytecode is required by psvm
add_dependencies(psvm generate_psvmjs_c)


target_sources(psvm
        PRIVATE
        psvm.cpp
        psvmjs.c
)