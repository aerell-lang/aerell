/*
 * Copyright (c) 2025, The Aerell Language Project Authors
 * All rights reserved.
 *
 * This file is part of the Aerell Language project.
 *
 * Licensed under the BSD 3-Clause License.
 * You may obtain a copy of the License at:
 * https://github.com/aerell-lang/aerell/blob/main/LICENSE
 *
 * File: source_file.h
 * Description: Declaration function and struct for source file.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-14
 */

#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include <stddef.h>

typedef struct source_file
{
    const char* file_path;
    const char* buffer;
    size_t buffer_size;
    size_t* line_offsets;
    size_t line_count;
} source_file_t;

source_file_t* source_file_create(const char* file_path);

void source_file_print(source_file_t* source_file);

void source_file_free(source_file_t* source_file);

#endif