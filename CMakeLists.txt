cmake_minimum_required(VERSION 4.0.0)

project(
    "Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CCache
find_program(CCACHE_PROGRAM "ccache")

if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache found and enable")
else()
    message(STATUS "ccache not found")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Enable all common warnings and debug information for better reports
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

    # Add ASan flags for all targets in the project
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# Aerell
set(AERELL aerell)

file(
    GLOB_RECURSE
    AERELL_SRC_FILES
    "src/aerell/*.cpp"
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