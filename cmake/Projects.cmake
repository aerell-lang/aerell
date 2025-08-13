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
# File: Projects.cmake
# Description: Contains the list of all subprojects and their inclusion order.
# Author: Fern Aerell fernaerell2020@gmail.com
# Created: 2025-08-13
# -----------------------------------------------------------------------------

set(
    SUBPROJECT_DIR_NAMES
    "aerell"
    "argp"
)

foreach(SUBPROJECT_DIR_NAME ${SUBPROJECT_DIR_NAMES})
    add_subdirectory(${SUBPROJECT_DIR_NAME})
endforeach()