/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef ASTS_H
#define ASTS_H

#include <stddef.h>

#include "aerell/parser/ast/ast.h"

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