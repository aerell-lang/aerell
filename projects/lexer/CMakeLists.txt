file(
    GLOB_RECURSE
    "LEXER_SOURCE_FILES"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(
    "lexer"
    SHARED
    ${LEXER_SOURCE_FILES}
)

target_include_directories(
    "lexer"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/private"
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/public"
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/interface"
)