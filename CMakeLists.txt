# -----------------------------------------------------------------------------
# Copyright (c) 2025, The Aerell Language Project Authors
# All rights reserved.
# 
# This file is part of the Aerell Language project.
# 
# Licensed under the BSD 3-Clause License. 
# You may obtain a copy of the License at:
# https://github.com/aerell-lang/aerell/blob/main/LICENSE
# 
# File: CMakeLists.txt
# Description: Build configuration for the root project.
# Author: Fern Aerell fernaerell2020@gmail.com
# Created: 2025-08-12
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 4.0.3)

project(
    "aerell"
    VERSION 1.0.0
    LANGUAGES C
)

# C Standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug or Release)" FORCE)
endif()

# Flags for Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wextra")

# Flags for Release
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -flto -funroll-loops -fomit-frame-pointer")

# Linker flags Release
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")

# aerell
file(
    GLOB_RECURSE
    "${PROJECT_NAME}_SOURCE_FILES"
    "src/*.c"
)

add_executable(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "include"
    "build/include"
)

configure_file(
    "include/config.h.in"
    "include/config.h"
)