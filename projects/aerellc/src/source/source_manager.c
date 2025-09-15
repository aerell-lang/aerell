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
 * File: source_manager.c
 * Description: Definition function and struct for source manager.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-15
 */

#include <stdlib.h>

#include "aerellc/source/source_manager.h"

source_manager_t* source_manager_create()
{
    source_manager_t* source_manager = malloc(sizeof(source_manager_t));
    if(!source_manager) return NULL;
    source_manager->capacity = 0;
    source_manager->length = 0;
    source_manager->files = NULL;
    return source_manager;
}

int source_manager_add(source_manager_t* source_manager, source_file_t* source_file)
{
    if(!source_manager || !source_file) return 0;
    if(source_manager->length >= source_manager->capacity)
    {
        size_t new_capacity = (source_manager->capacity == 0) ? 2 : source_manager->capacity * 2;
        source_file_t** new_files = realloc(source_manager->files, new_capacity * sizeof(source_file_t*));
        if(!new_files) return 0;
        source_manager->files = new_files;
        source_manager->capacity = new_capacity;
    }
    source_manager->files[source_manager->length++] = source_file;
    return 1;
}

int source_manager_shrink(source_manager_t* source_manager)
{
    if(!source_manager) return 0;
    if(source_manager->length == 0) return 1;
    source_file_t** new_files = realloc(source_manager->files, source_manager->length * sizeof(source_file_t*));
    if(!new_files) return 0;
    source_manager->files = new_files;
    source_manager->capacity = source_manager->length;
    return 1;
}

void source_manager_free(source_manager_t* source_manager)
{
    if(!source_manager) return;
    if(source_manager->files)
    {
        for(size_t i = 0; i < source_manager->length; i++)
        {
            source_file_free(source_manager->files[i]);
            source_manager->files[i] = NULL;
        }
    }
    source_manager->capacity = 0;
    source_manager->length = 0;
    free(source_manager->files);
    source_manager->files = NULL;
    free(source_manager);
}