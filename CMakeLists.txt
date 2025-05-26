cmake_minimum_required(VERSION 3.14)
project(json-lines-tools VERSION 0.0.0)
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_CPM "Enable CPM package manager" OFF)

if (ENABLE_CPM)
    include(cmake/get_cpm.cmake)
    CPMAddPackage("gh:simdjson/simdjson@3.12.3")
else ()
    find_package(simdjson REQUIRED)
endif ()
add_executable(jsonl2json jsonl2json.cpp)
target_link_libraries(jsonl2json PRIVATE simdjson::simdjson)
target_compile_options(jsonl2json PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
install(TARGETS jsonl2json)

SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "43220762+hwse@users.noreply.github.com")
SET(CPACK_PACKAGE_CONTACT "hwse")
IF (ENABLE_CPM)
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS " ")
else ()
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsimdjson19")
endif ()
include(CPack)