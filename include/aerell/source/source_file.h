/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include <stddef.h>

typedef struct source_file
{
    const char* file_path;
    const unsigned char* buffer;
    size_t buffer_size;
    size_t* line_offsets;
    size_t line_count;
} source_file_t;

source_file_t* source_file_create(const char* file_path);

void source_file_print(source_file_t* source_file);

void source_file_free(source_file_t* source_file);

#endif