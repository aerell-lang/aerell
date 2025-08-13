set(
    PROJECT_DIR_NAMES
    "aerell"
    "argp"
)

foreach(PROJECT_DIR_NAME ${PROJECT_DIR_NAMES})
    add_subdirectory(${PROJECT_DIR_NAME})
endforeach()