// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_AST_AST_TYPE_H
#define AERELL_AST_AST_TYPE_H

typedef enum
{
    AST_TYPE_EOF,
    AST_TYPE_INTL,
} ast_type_t;

const char* ast_type_to_string(ast_type_t t);

#endif