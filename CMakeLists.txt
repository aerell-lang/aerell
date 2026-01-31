cmake_minimum_required(VERSION 4.0.0)

project(
    "Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    set(CMAKE_C_AND_CXX_FLAGS "-g -O0 \
        -Wall -Wextra -Werror \
        -fno-omit-frame-pointer \
        -fsanitize=address \
        -fsanitize=undefined \
        -fsanitize-address-use-after-scope")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_AND_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_AND_CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_AND_CXX_FLAGS "-O3 -Os \
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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_AND_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_AND_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
        -static \
        -Wl,-s \
        -Wl,--build-id=none \
        -Wl,--gc-sections \
        -Wl,--strip-all \
        -Wl,--as-needed")
endif()

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs support core native passes)

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

target_link_libraries(
    ${AERELL} 
    PRIVATE
    ${llvm_libs}
)

# Installation
install(
    TARGETS
    ${AERELL}
)