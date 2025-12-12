file(
    GLOB_RECURSE
    SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
)

add_library(
    "support"
    ${SRC_FILES}
)

target_include_directories(
    "support"
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include/public"
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/private"
    INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/include/interface"
)