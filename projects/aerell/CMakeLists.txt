file(
    GLOB_RECURSE
    SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
)

add_executable(
    "aerell"
    ${SRC_FILES}
)

target_link_libraries(
    "aerell"
    "lexer"
)