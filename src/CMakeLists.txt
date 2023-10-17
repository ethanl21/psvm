
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