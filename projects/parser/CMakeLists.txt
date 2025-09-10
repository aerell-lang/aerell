file(
    GLOB_RECURSE
    "PARSER_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(
    "parser"
    SHARED
    ${PARSER_SOURCE_FILES}
)

target_include_directories(
    "parser"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interface"
)

target_link_libraries(
    "parser"
    PRIVATE
    "lexer"
)