cmake_minimum_required(VERSION 4.0.3)

project(
    "aerell"
    VERSION 0.0.1
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
    GLOB_RECURSE 
    "${PROJECT_NAME}_SOURCE_FILES"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(
    ${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCE_FILES}
)