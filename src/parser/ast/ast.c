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
 * Description: AST function implementation.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser/ast/ast_type.h"
#include "parser/ast/asts.h"
#include "parser/ast/ast.h"

ASTStr* ast_str_create(Token* value)
{
    ASTStr* str = malloc(sizeof(ASTStr));
    str->base.type = AST_TYPE_STR;
    str->value = value;
    return str;
}

void print_ast_str(ASTStr* str)
{
    if(str != NULL)
    {
        printf("[%i] %s\n", str->base.type, str->value->content);
    }
}

void ast_str_free(ASTStr* str)
{
    if(str != NULL) free(str);
}

ASTId* ast_id_create(Token* value)
{
    ASTId* id = malloc(sizeof(ASTId));
    id->base.type = AST_TYPE_ID;
    id->value = value;
    id->children = asts_create();
    return id;
}

void print_ast_id(ASTId* id)
{
    if(id != NULL)
    {
        printf("[%i] %s\n", id->base.type, id->value->content);
        print_asts(id->children, 1);
    }
}

void ast_id_free(ASTId* id)
{
    if(id->children != NULL)
    {
        asts_free(id->children);
        id->children = NULL;
    }
    if(id != NULL) free(id);
}

void print_ast(AST* ast)
{
    if(ast->type == AST_TYPE_STR)
    {
        print_ast_str((ASTStr*)ast);
    }
    else if(ast->type == AST_TYPE_ID)
    {
        print_ast_id((ASTId*)ast);
    }
}

void ast_free(AST* ast)
{
    if(ast != NULL)
    {
        if(ast->type == AST_TYPE_STR)
        {
            ASTStr* str = (ASTStr*)ast;
            ast_str_free(str);
            str = NULL;
        }
        else if(ast->type == AST_TYPE_ID)
        {
            ASTId* id = (ASTId*)ast;
            ast_id_free(id);
            id = NULL;
        }
    }
}