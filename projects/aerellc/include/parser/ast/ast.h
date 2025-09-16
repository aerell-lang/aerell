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
 * Description: AST struct and function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "ast_type.h"
#include "lexer/token/token.h"

typedef struct asts asts_t;

typedef struct ast
{
    ast_type_t type;
} ast_t;

typedef struct ast_data_type
{
    ast_t base;
    token_t* value;
    bool is_pointer;
} ast_data_type_t;

typedef struct ast_func_param
{
    ast_t base;
    token_t* name;
    ast_data_type_t* data_type;
} ast_func_param_t;

typedef struct ast_func
{
    ast_t base;
    token_t* name;
    asts_t* params;
    bool is_variadic;
    ast_data_type_t* return_data_type;
} ast_func_t;

ast_data_type_t* ast_data_type_create(token_t* value, bool is_pointer);

ast_func_param_t* ast_func_param_create(token_t* name, ast_data_type_t* data_type);

ast_func_t* ast_func_create(token_t* name, asts_t* params, bool is_variadic, ast_data_type_t* return_data_type);

void ast_data_type_print(ast_data_type_t* ast, size_t indent);

void ast_func_param_print(ast_func_param_t* ast, size_t indent);

void ast_func_print(ast_func_t* ast, size_t indent);

void ast_print(ast_t* ast, size_t indent);

void ast_data_type_free(ast_data_type_t* ast);

void ast_func_param_free(ast_func_param_t* ast);

void ast_func_free(ast_func_t* ast);

void ast_free(ast_t* ast);

#endif