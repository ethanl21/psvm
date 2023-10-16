cmake_minimum_required(VERSION 3.26)
project(quickjs LANGUAGES C)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/quickjspp/VERSION" version)

set(version_header "#define QUICKJS_VERSION \"${version}\"\n")
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/quickjspp/quickjs-version.h" "${version_header}")

set(quickjs_src quickjs.c libbf.c libunicode.c libregexp.c cutils.c quickjs-libc.c)

set(prefix "${CMAKE_CURRENT_SOURCE_DIR}/quickjspp/")
set(prefixed_srcs)

foreach(item ${quickjs_src})
    string(CONCAT modified_item "${prefix}" "${item}")
    list(APPEND prefixed_srcs ${modified_item})
endforeach()

add_library(quickjs ${prefixed_srcs})
target_compile_definitions(quickjs PRIVATE CONFIG_BIGNUM _GNU_SOURCE)
target_compile_definitions(quickjs PUBLIC JS_STRICT_NAN_BOXING)

add_library(quickjs-dumpleaks ${prefixed_srcs})
target_compile_definitions(quickjs-dumpleaks PRIVATE CONFIG_BIGNUM _GNU_SOURCE DUMP_LEAKS=1)
target_compile_definitions(quickjs-dumpleaks PUBLIC JS_STRICT_NAN_BOXING)

add_executable(qjsc "${CMAKE_CURRENT_SOURCE_DIR}/quickjspp/qjsc.c")
target_link_libraries(qjsc PRIVATE quickjs)

if(UNIX)
    find_package(Threads)
    target_link_libraries(quickjs ${CMAKE_DL_LIBS} m Threads::Threads)
    target_link_libraries(quickjs-dumpleaks ${CMAKE_DL_LIBS} m Threads::Threads)
endif()