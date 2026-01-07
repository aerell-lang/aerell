// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/ast/ast.h"

void _debug_ast(const ast_t* ast)
{
    switch(ast->type)
    {
    case AST_TYPE_INTL: printf("%s %d\n", ast_type_to_string(ast->type), ast->value); break;
    case AST_TYPE_EOF: printf("%s\n", ast_type_to_string(ast->type)); break;
    }
}

void debug_ast(const ast_t* ast)
{
    if(ast == NULL) return;

    while(ast->type != AST_TYPE_EOF)
    {
        _debug_ast(ast);
        ast++;
    }

    _debug_ast(ast);
}