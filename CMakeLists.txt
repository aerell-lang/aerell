cmake_minimum_required(VERSION 4.0.0)

project(
    "The Aerell Programming Language"
    VERSION 0.0.0
    LANGUAGES C CXX
)

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

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    llvm_map_components_to_libnames(
        "LLVM_LIBS"
        # libraries :
        "core"
        "support"
        "passes"
        "native"
        "lto"
        "windowsmanifest"
        "windowsdriver"
        "libdriver"
        "orcjit"
    )
else()
    set(
        "LLVM_LIBS"
        # libraries :
        "LLVM-21"
    )
endif()

# LLD
find_package(LLD REQUIRED CONFIG)

message(STATUS "Found LLD ${LLD_VERSION}")

set(
    "LLD_LIBS"
    # libraries :
    "lldcommon"
    "lldcoff"
    # "lldelf"
    # "lldmacho"
    "lldmingw"
    # "lldwasm"
)

# Aerell Lib
set("AERELL_LIB_PROJECT_NAME" "aerell_lib")
set("AERELL_LIB_STATIC_PROJECT_NAME" "${AERELL_LIB_PROJECT_NAME}_static")
set("AERELL_LIB_SHARED_PROJECT_NAME" "${AERELL_LIB_PROJECT_NAME}_shared")

file(
    GLOB_RECURSE
    "AERELL_LIB_SRC_FILES"
    # "${CMAKE_CURRENT_LIST_DIR}/src/aerell/library/*.S"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/library/*.c"
)

add_library(
    ${AERELL_LIB_STATIC_PROJECT_NAME}
    STATIC
    ${AERELL_LIB_SRC_FILES}
)

add_library(
    ${AERELL_LIB_SHARED_PROJECT_NAME}
    SHARED
    ${AERELL_LIB_SRC_FILES}
)

set(
    "AERELL_LIB_COMPILE_AND_LINK_OPTIONS"
    "-ffreestanding"
    "-nostdlib"
    "-fno-builtin"
    "-fno-sanitize=address,undefined"
)

target_compile_options(
    ${AERELL_LIB_STATIC_PROJECT_NAME}
    PRIVATE
    ${AERELL_LIB_COMPILE_AND_LINK_OPTIONS}
)

target_compile_options(
    ${AERELL_LIB_SHARED_PROJECT_NAME}
    PRIVATE
    ${AERELL_LIB_COMPILE_AND_LINK_OPTIONS}
)

target_link_options(
    ${AERELL_LIB_STATIC_PROJECT_NAME}
    PRIVATE
    ${AERELL_LIB_COMPILE_AND_LINK_OPTIONS}
)

target_link_options(
    ${AERELL_LIB_SHARED_PROJECT_NAME}
    PRIVATE
    ${AERELL_LIB_COMPILE_AND_LINK_OPTIONS}
)

set_target_properties(${AERELL_LIB_STATIC_PROJECT_NAME} PROPERTIES OUTPUT_NAME "aerell")
set_target_properties(${AERELL_LIB_SHARED_PROJECT_NAME} PROPERTIES OUTPUT_NAME "aerell")

# Aerell
set("AERELL_PROJECT_NAME" "aerell")

file(
    GLOB_RECURSE
    "AERELL_SRC_FILES"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/compiler/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/linker/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/support/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/cli/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/aerell/app/*.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_source_files_properties(
        "${CMAKE_CURRENT_LIST_DIR}/src/aerell/compiler/compiler_jit.cpp"
        PROPERTIES
        COMPILE_OPTIONS "-fno-sanitize=address,undefined"
    )
endif()

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
    ${LLVM_LIBS}
    ${LLD_LIBS}
)

# Install
install(
    TARGETS 
    ${AERELL_LIB_STATIC_PROJECT_NAME}
    ${AERELL_LIB_SHARED_PROJECT_NAME}
    ${AERELL_PROJECT_NAME}
)

install(
    FILES
    "LICENSE"
    DESTINATION
    ${CMAKE_INSTALL_PREFIX}
)

install(
    DIRECTORY
    "${CMAKE_CURRENT_LIST_DIR}/src/std"
    DESTINATION
    "${CMAKE_INSTALL_PREFIX}/src"
)