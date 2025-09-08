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
 * Description: List of AST struct and function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef ASTS_H
#define ASTS_H

#include <stddef.h>

#include "parser/ast/ast.h"

typedef struct ASTs
{
    size_t length;
    size_t capacity;
    AST** data;
} ASTs;

ASTs* asts_create();

void print_asts(ASTs* asts, size_t indent);

int asts_add(ASTs* asts, AST* ast);

void asts_free(ASTs* asts);

int asts_shrink(ASTs* asts);

#endif