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

#include "parser/ast/asts.h"

ASTs* asts_create()
{
    ASTs* asts = malloc(sizeof(ASTs));
    if(!asts) return NULL;
    asts->capacity = 0;
    asts->length = 0;
    asts->data = NULL;
    return asts;
}

void asts_print(ASTs* asts, size_t indent)
{
    if(!asts) return;
    for(size_t i = 0; i < asts->length; i++) ast_print(asts->data[i], indent);
}

int asts_add(ASTs* asts, AST* ast)
{
    if(!asts || !ast) return 0;
    if(asts->length >= asts->capacity)
    {
        size_t new_capacity = (asts->capacity == 0) ? 2 : asts->capacity * 2;
        AST** data_temp = realloc(asts->data, new_capacity * sizeof(AST*));
        if(!data_temp) return 0;
        asts->data = data_temp;
        asts->capacity = new_capacity;
    }
    asts->data[asts->length++] = ast;
    return 1;
}

void asts_free(ASTs* asts)
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

int asts_shrink(ASTs* asts)
{
    if(!asts) return 0;

    if(asts->length == 0)
    {
        asts_free(asts);
        return 1;
    }

    AST** data_temp = realloc(asts->data, asts->length * sizeof(AST*));
    if(!data_temp) return 0;

    asts->data = data_temp;
    asts->capacity = asts->length;

    return 1;
}