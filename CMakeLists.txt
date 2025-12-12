cmake_minimum_required(VERSION 4.2)

project(
    "The Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C CXX
)

# C Standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug or Release)" FORCE)
endif()

# Flags for Debug
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Wextra -Werror -fsanitize=address")

# Flags for Release
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -ffast-math -finline-functions -fdata-sections -ffunction-sections -Os -DNDEBUG -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -g0")

# Linker flags Release - strip and garbage collection
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections -Wl,--strip-all -Wl,--as-needed")

# Optimasi interprocedural (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# CCACHE
find_program(CCACHE_PROGRAM "ccache")
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

# Add projects folder
add_subdirectory("projects")