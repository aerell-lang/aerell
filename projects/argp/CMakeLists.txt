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
# Description: Build configuration for the argp project.
# Author: Fern Aerell fernaerell2020@gmail.com
# Created: 2025-08-12
# -----------------------------------------------------------------------------

set(PROJECT_NAME "argp")

file(
    GLOB_RECURSE 
    "${PROJECT_NAME}_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${${PROJECT_NAME}_SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interface"
)