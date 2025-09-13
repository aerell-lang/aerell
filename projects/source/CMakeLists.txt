file(
    GLOB_RECURSE
    "SOURCE_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(
    "source"
    STATIC
    ${SOURCE_SOURCE_FILES}
)

target_include_directories(
    "source"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interface"
)