file(
    GLOB_RECURSE
    "AERELLC_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(
    "aerellc"
    ${AERELLC_SOURCE_FILES}
)

target_include_directories(
    "aerellc"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
)