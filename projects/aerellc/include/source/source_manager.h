/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
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