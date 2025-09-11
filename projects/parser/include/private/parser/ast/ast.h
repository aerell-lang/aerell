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

#include <lexer/lexer.h>

typedef struct ASTs ASTs;

typedef struct AST
{
    int type;
} AST;

typedef struct ASTDataType
{
    AST base;
    Token* value;
    bool is_pointer;
} ASTDataType;

typedef struct ASTFuncParam
{
    AST base;
    ASTDataType* data_type;
    Token* name;
} ASTFuncParam;

typedef struct ASTFuncDecl
{
    AST base;
    Token* name;
    ASTs* params;
    bool is_variadic;
    ASTDataType* return_data_type;
} ASTFuncDecl;

ASTDataType* ast_data_type_create(Token* value, bool is_pointer);

ASTFuncParam* ast_func_param_create(ASTDataType* data_type, Token* name);

ASTFuncDecl* ast_func_decl_create(Token* name, ASTs* params, bool is_variadic, ASTDataType* return_data_type);

void ast_data_type_print(ASTDataType* ast, size_t indent);

void ast_func_param_print(ASTFuncParam* ast, size_t indent);

void ast_func_decl_print(ASTFuncDecl* ast, size_t indent);

void ast_print(AST* ast, size_t indent);

void ast_data_type_free(ASTDataType* ast);

void ast_func_param_free(ASTFuncParam* ast);

void ast_func_decl_free(ASTFuncDecl* ast);

void ast_free(AST* ast);

#endif