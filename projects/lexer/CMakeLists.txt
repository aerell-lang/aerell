file(
    GLOB_RECURSE
    SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
)

add_library(
    "lexer"
    ${SRC_FILES}
)

target_include_directories(
    "lexer"
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include/interface"
)

target_link_libraries(
    "lexer"
    "core"
)