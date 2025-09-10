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
    Token* id;
} ASTFuncParam;

typedef struct ASTFuncDecl
{
    AST base;
    ASTDataType* ret_type;
    Token* id;
    bool is_variadic;
    ASTFuncParam** params;
} ASTFuncDecl;

typedef struct ASTFuncArg
{
    AST base;
    Token* value;
} ASTFuncArg;

typedef struct ASTFuncCall
{
    AST base;
    Token* id;
    ASTFuncArg** args;
} ASTFuncCall;

ASTDataType* ast_data_type_create();
ASTFuncParam* ast_func_param_create();
ASTFuncDecl* ast_func_decl_create();
ASTFuncArg* ast_func_arg_create();
ASTFuncCall* ast_func_call_create();

void ast_data_type_free(ASTDataType* ast);
void ast_func_param_free(ASTFuncParam* ast);
void ast_func_decl_free(ASTFuncDecl* ast);
void ast_func_arg_free(ASTFuncArg* ast);
void ast_func_call_free(ASTFuncCall* ast);
void ast_free(AST* ast);

#endif