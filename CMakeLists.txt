cmake_minimum_required(VERSION 4.0.0)

project(
    "Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C
)

# CCache
find_program(CCACHE_PROGRAM "ccache")

if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache found and enable")
else()
    message(STATUS "ccache not found")
endif()

# Aerell
set(AERELL aerell)

file(
    GLOB_RECURSE
    AERELL_SRC_FILES
    "src/aerell/*.c"
)

add_executable(
    ${AERELL}
    ${AERELL_SRC_FILES}
)

target_include_directories(
    ${AERELL}
    PRIVATE
    "include"
)

# Installation
install(
    TARGETS
    ${AERELL}
)