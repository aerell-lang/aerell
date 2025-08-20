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

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("projects")