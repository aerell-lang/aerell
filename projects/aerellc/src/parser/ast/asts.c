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
 * File: asts.c
 * Description: List of AST function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>
#include <stdlib.h>

#include "aerellc/parser/ast/asts.h"

asts_t* asts_create()
{
    asts_t* asts = malloc(sizeof(asts_t));
    if(!asts) return NULL;
    asts->capacity = 0;
    asts->length = 0;
    asts->data = NULL;
    return asts;
}

void asts_print(asts_t* asts, size_t indent)
{
    if(!asts) return;
    for(size_t i = 0; i < asts->length; i++) ast_print(asts->data[i], indent);
}

int asts_add(asts_t* asts, ast_t* ast)
{
    if(!asts || !ast) return 0;
    if(asts->length >= asts->capacity)
    {
        size_t new_capacity = (asts->capacity == 0) ? 2 : asts->capacity * 2;
        ast_t** new_data = realloc(asts->data, new_capacity * sizeof(ast_t*));
        if(!new_data) return 0;
        asts->data = new_data;
        asts->capacity = new_capacity;
    }
    asts->data[asts->length++] = ast;
    return 1;
}

int asts_shrink(asts_t* asts)
{
    if(!asts) return 0;
    if(asts->length == 0) return 1;

    ast_t** new_data = realloc(asts->data, asts->length * sizeof(ast_t*));
    if(!new_data) return 0;

    asts->data = new_data;
    asts->capacity = asts->length;

    return 1;
}

void asts_free(asts_t* asts)
{
    if(!asts) return;
    asts->length = 0;
    asts->capacity = 0;
    for(size_t i = 0; i < asts->length; i++)
    {
        if(asts->data[i])
        {
            ast_free(asts->data[i]);
            asts->data[i] = NULL;
        }
    }
    if(asts->data)
    {
        free(asts->data);
        asts->data = NULL;
    }
    free(asts);
}