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
 * File: source_manager.h
 * Description: Declaration function and struct for source manager.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-15
 */

#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include "source/source_file.h"

typedef struct source_manager
{
    size_t length;
    size_t capacity;
    source_file_t** files;
} source_manager_t;

source_manager_t* source_manager_create();

int source_manager_add(source_manager_t* source_manager, source_file_t* source_file);

int source_manager_shrink(source_manager_t* source_manager);

void source_manager_free(source_manager_t* source_manager);

#endif