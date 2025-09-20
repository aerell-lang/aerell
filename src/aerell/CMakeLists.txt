file(
    GLOB_RECURSE
    "AERELL_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(
    "aerell"
    ${AERELL_SOURCE_FILES}
)