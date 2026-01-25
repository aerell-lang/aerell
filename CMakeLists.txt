cmake_minimum_required(VERSION 4.0.0)

project(
    "Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ccache
find_program(CCACHE_PROGRAM "ccache")

if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache found and enable")
else()
    message(STATUS "ccache not found")
endif()

# Build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
        -g -O0 \
        -Wall -Wextra -Werror \
        -fno-omit-frame-pointer \
        -fsanitize=address \
        -fsanitize=undefined \
        -fsanitize-address-use-after-scope")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
        -O3 -Os \
        -march=native -mtune=native \
        -flto \
        -funroll-loops \
        -finline-functions \
        -fomit-frame-pointer \
        -ffast-math \
        -fdata-sections -ffunction-sections \
        -DNDEBUG \
        -fno-stack-protector \
        -fno-unwind-tables \
        -fno-asynchronous-unwind-tables \
        -g0")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
        -static \
        -Wl,-s \
        -Wl,--build-id=none \
        -Wl,--gc-sections \
        -Wl,--strip-all \
        -Wl,--as-needed")
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