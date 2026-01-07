// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_AST_AST_H
#define AERELL_AST_AST_H

#include <stddef.h>

#include "aerell/ast/ast_type.h"

typedef struct
{
    ast_type_t type;
    int value;
} ast_t;

void debug_ast(const ast_t* ast);

#endif