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
 * File: ast.h
 * Description: AST struct and function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "lexer/token/token.h"

typedef struct ASTs ASTs;

typedef struct AST
{
    int type;
} AST;

typedef struct ASTStr
{
    AST base;
    Token* value;
} ASTStr;

typedef struct ASTId
{
    AST base;
    Token* value;
    ASTs* children;
} ASTId;

ASTStr* ast_str_create(Token* value);

void print_ast_str(ASTStr* str);

void ast_str_free(ASTStr* str);

ASTId* ast_id_create(Token* value);

void print_ast_id(ASTId* id);

void ast_id_free(ASTId* id);

void print_ast(AST* ast);

void ast_free(AST* ast);

#endif