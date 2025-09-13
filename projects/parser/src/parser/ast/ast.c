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
 * File: ast.c
 * Description: AST function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser/ast/ast.h"
#include "parser/ast/ast_type.h"
#include "parser/ast/asts.h"

#define AST_INDENT 4

ASTDataType* ast_data_type_create(Token* value, bool is_pointer)
{
    if(!value) return NULL;
    ASTDataType* ast = malloc(sizeof(ASTDataType));
    if(!ast) return NULL;
    ast->base.type = AST_DATA_TYPE;
    ast->value = value;
    ast->is_pointer = is_pointer;
    return ast;
}

ASTFuncParam* ast_func_param_create(Token* name, ASTDataType* data_type)
{
    if(!name || !data_type) return NULL;
    ASTFuncParam* ast = malloc(sizeof(ASTFuncParam));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC_PARAM;
    ast->name = name;
    ast->data_type = data_type;
    return ast;
}

ASTFunc* ast_func_create(Token* name, ASTs* params, bool is_variadic, ASTDataType* return_data_type)
{
    if(!name || !params) return NULL;
    ASTFunc* ast = malloc(sizeof(ASTFunc));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC;
    ast->name = name;
    ast->params = params;
    ast->is_variadic = is_variadic;
    ast->return_data_type = return_data_type;
    return ast;
}

void ast_data_type_print(ASTDataType* ast, size_t indent)
{
    if(!ast || !ast->value) return;
    printf("%*sASTDataType\n", (int)indent, "");
    printf("%*s    value: %s\n", (int)indent, "", token_get_content(ast->value));
    printf("%*s    is_pointer: %i\n", (int)indent, "", ast->is_pointer);
}

void ast_func_param_print(ASTFuncParam* ast, size_t indent)
{
    if(!ast || !ast->data_type || !ast->name) return;
    printf("%*sASTFuncParam\n", (int)indent, "");
    printf("%*s    name: %s\n", (int)indent, "", token_get_content(ast->name));
    printf("%*s    data_type: \n", (int)indent, "");
    ast_data_type_print(ast->data_type, indent + AST_INDENT);
}

void ast_func_print(ASTFunc* ast, size_t indent)
{
    if(!ast || !ast->name || !ast->params) return;
    printf("%*sASTFunc\n", (int)indent, "");
    printf("%*s    name: %s\n", (int)indent, "", token_get_content(ast->name));
    printf("%*s    params: \n", (int)indent, "");
    asts_print(ast->params, indent + AST_INDENT);
    printf("%*s    is_variadic: %i\n", (int)indent, "", ast->is_variadic);
    printf("%*s    ret_type: \n", (int)indent, "");
    ast_data_type_print(ast->return_data_type, indent + AST_INDENT);
}

void ast_print(AST* ast, size_t indent)
{
    if(!ast) return;
    else if(ast->type == AST_DATA_TYPE)
        ast_data_type_print((ASTDataType*)ast, indent);
    else if(ast->type == AST_FUNC_PARAM)
        ast_func_param_print((ASTFuncParam*)ast, indent);
    else if(ast->type == AST_FUNC)
        ast_func_print((ASTFunc*)ast, indent);
}

void ast_data_type_free(ASTDataType* ast)
{
    if(!ast) return;
    free(ast);
}

void ast_func_param_free(ASTFuncParam* ast)
{
    if(!ast) return;
    ast_data_type_free(ast->data_type);
    free(ast);
}

void ast_func_free(ASTFunc* ast)
{
    if(!ast) return;
    asts_free(ast->params);
    ast_data_type_free(ast->return_data_type);
    free(ast);
}

void ast_free(AST* ast)
{
    if(!ast) return;
    else if(ast->type == AST_DATA_TYPE)
        ast_data_type_free((ASTDataType*)ast);
    else if(ast->type == AST_FUNC_PARAM)
        ast_func_param_free((ASTFuncParam*)ast);
    else if(ast->type == AST_FUNC)
        ast_func_free((ASTFunc*)ast);
}