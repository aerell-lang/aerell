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
 * File: asts.h
 * Description: List of AST struct and function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef ASTS_H
#define ASTS_H

#include <stddef.h>

#include "parser/ast/ast.h"

typedef struct asts
{
    size_t length;
    size_t capacity;
    ast_t** data;
} asts_t;

asts_t* asts_create();

void asts_print(asts_t* asts, size_t indent);

int asts_add(asts_t* asts, ast_t* ast);

int asts_shrink(asts_t* asts);

void asts_free(asts_t* asts);

#endif