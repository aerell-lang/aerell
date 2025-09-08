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
 * Description: List of AST function implementation.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser/ast/asts.h"

ASTs* asts_create()
{
    ASTs* asts = malloc(sizeof(ASTs));
    asts->capacity = 0;
    asts->length = 0;
    asts->data = NULL;
    return asts;
}

void print_asts(ASTs* asts, size_t indent)
{
    if(asts != NULL)
    {
        for(size_t i = 0; i < asts->length; i++)
        {
            printf("%*s", (int)(indent * 3), "");
            print_ast(asts->data[i]);
        }
    }
}

int asts_add(ASTs* asts, AST* ast)
{
    if(asts->length >= asts->capacity)
    {
        size_t new_capacity = (asts->capacity == 0) ? 2 : asts->capacity * 2;
        AST** data_temp = realloc(asts->data, new_capacity * sizeof(AST*));
        if(!data_temp)
        {
            printf("Failed to realloc data to data_temp.\n");
            return 0;
        }
        asts->data = data_temp;
        asts->capacity = new_capacity;
    }
    asts->data[asts->length++] = ast;
    return 1;
}

void asts_free(ASTs* asts)
{
    asts->length = 0;
    asts->capacity = 0;
    for(size_t i = 0; i < asts->length; i++)
    {
        if(asts->data[i] != NULL)
        {
            ast_free(asts->data[i]);
            asts->data[i] = NULL;
        }
    }
    if(asts->data != NULL)
    {
        free(asts->data);
        asts->data = NULL;
    }
    if(asts != NULL) free(asts);
}

int asts_shrink(ASTs* asts)
{
    if(asts->length == 0)
    {
        asts_free(asts);
        return 1;
    }

    AST** data_temp = realloc(asts->data, asts->length * sizeof(AST*));
    if(!data_temp)
    {
        printf("Failed to realloc data to data_temp.\n");
        return 0;
    }

    asts->data = data_temp;
    asts->capacity = asts->length;

    return 1;
}