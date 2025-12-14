cmake_minimum_required(VERSION 4.2)

project(
    "The Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C CXX ASM
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
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall -Werror -Wextra -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Werror -Wextra -fsanitize=address")

# Flags for Release
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static -O3 -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -ffast-math -finline-functions -fdata-sections -ffunction-sections -Os -DNDEBUG -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -g0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static -O3 -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -ffast-math -finline-functions -fdata-sections -ffunction-sections -Os -DNDEBUG -fno-stack-protector -fno-unwind-tables -fno-asynchronous-unwind-tables -g0")

# Linker flags Release - strip and garbage collection
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Wl,--gc-sections -Wl,--strip-all -Wl,--as-needed")

# Set asm command-line rule
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <FLAGS> -c <SOURCE> -o <OBJECT>")

# Optimasi interprocedural (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# CCACHE
find_program(CCACHE_PROGRAM "ccache")
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(
    "llvm_libs"
    # libraries :
    "core"
    "support"
    "passes"
    "native"
    "lto"
    "windowsmanifest"
    "windowsdriver"
    "libdriver"
)

# LLD
find_package(LLD REQUIRED CONFIG)

message(STATUS "Found LLD ${LLD_VERSION}")

set(
    "lld_libs"
    # libraries :
    "lldcommon"
    "lldcoff"
    "lldelf"
    "lldmacho"
    "lldmingw"
    "lldwasm"
)

# Aerell
set("AERELL_PROJECT_NAME" "aerell")

file(
    GLOB_RECURSE
    "AERELL_SRC_FILES"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/compiler/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/linker/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/support/*.cpp"
    "main.cpp"
)

add_executable(
    ${AERELL_PROJECT_NAME}
    ${AERELL_SRC_FILES}
)

target_include_directories(
    ${AERELL_PROJECT_NAME}
    SYSTEM
    PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${LLD_INCLUDE_DIRS}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_compile_definitions(
    ${AERELL_PROJECT_NAME}
    PRIVATE
    ${LLVM_DEFINITIONS_LIST}
)

target_link_directories(
    ${AERELL_PROJECT_NAME}
    PRIVATE
    ${LLVM_LIBRARY_DIRS}
)

target_link_libraries(
    ${AERELL_PROJECT_NAME}
    PRIVATE
    ${llvm_libs}
    ${lld_libs}
)

# Aerell Lib
set("AERELL_LIB_PROJECT_NAME" "aerell_lib")

file(
    GLOB_RECURSE
    "AERELL_LIB_SRC_FILES"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/library/*.s"
)

add_library(
    ${AERELL_LIB_PROJECT_NAME}
    STATIC
    ${AERELL_LIB_SRC_FILES}
)

set_target_properties(${AERELL_LIB_PROJECT_NAME} PROPERTIES OUTPUT_NAME "aerell")

# Install
install(
    TARGETS 
    ${AERELL_PROJECT_NAME}
    ${AERELL_LIB_PROJECT_NAME}
)