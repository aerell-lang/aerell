file(
    GLOB_RECURSE
    "AERELL_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(
    "aerell"
    ${AERELL_SOURCE_FILES}
)

target_include_directories(
    "aerell"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interface"
)

target_link_libraries(
    "aerell"
    PRIVATE
    "lexer"
    "parser"
)