// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_PARSER_H
#define AERELL_PARSER_H

#include <stddef.h>

#include "lexer.h"
#include "aerell/ast/ast.h"

typedef struct
{
    lexer_t* lexer;
    ast_t* ast;
    size_t ast_capacity;
    size_t ast_size;
} parser_t;

void parser_init(parser_t* parser, lexer_t* lexer);

ast_t* parser_parse(parser_t* parser);

#endif