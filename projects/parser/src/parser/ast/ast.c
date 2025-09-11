#include <stdio.h>
#include <stdlib.h>

#include "parser/ast/ast.h"
#include "parser/ast/ast_type.h"
#include "parser/ast/asts.h"

#define AST_INDENT 4

ASTDataType* ast_data_type_create(Token* value, bool is_pointer)
{
    if(value == NULL) return NULL;
    ASTDataType* ast = malloc(sizeof(ASTDataType));
    if(ast == NULL) return NULL;
    ast->base.type = AST_DATA_TYPE;
    ast->value = value;
    ast->is_pointer = is_pointer;
    return ast;
}

ASTFuncParam* ast_func_param_create(ASTDataType* data_type, Token* name)
{
    if(data_type == NULL || name == NULL) return NULL;
    ASTFuncParam* ast = malloc(sizeof(ASTFuncParam));
    if(ast == NULL) return NULL;
    ast->base.type = AST_FUNC_PARAM;
    ast->data_type = data_type;
    ast->name = name;
    return ast;
}

ASTFuncDecl* ast_func_decl_create(Token* name, ASTs* params, bool is_variadic, ASTDataType* return_data_type)
{
    if(name == NULL || params == NULL) return NULL;
    ASTFuncDecl* ast = malloc(sizeof(ASTFuncDecl));
    if(ast == NULL) return NULL;
    ast->base.type = AST_FUNC_DECL;
    ast->name = name;
    ast->params = params;
    ast->is_variadic = is_variadic;
    ast->return_data_type = return_data_type;
    return ast;
}

void ast_data_type_print(ASTDataType* ast, size_t indent)
{
    if(ast == NULL || ast->value == NULL) return;
    printf("%*sASTDataType\n", (int)indent, "");
    printf("%*s    value: %s\n", (int)indent, "", token_get_content(ast->value));
    printf("%*s    is_pointer: %i\n", (int)indent, "", ast->is_pointer);
}

void ast_func_param_print(ASTFuncParam* ast, size_t indent)
{
    if(ast == NULL || ast->data_type == NULL || ast->name == NULL) return;
    printf("%*sASTFuncParam\n", (int)indent, "");
    printf("%*s    data_type: \n", (int)indent, "");
    ast_data_type_print(ast->data_type, indent + AST_INDENT);
    printf("%*s    name: %s\n", (int)indent, "", token_get_content(ast->name));
}

void ast_func_decl_print(ASTFuncDecl* ast, size_t indent)
{
    if(ast == NULL || ast->name == NULL || ast->params == NULL) return;
    printf("%*sASTFuncDecl\n", (int)indent, "");
    printf("%*s    name: %s\n", (int)indent, "", token_get_content(ast->name));
    printf("%*s    is_variadic: %i\n", (int)indent, "", ast->is_variadic);
    printf("%*s    params: \n", (int)indent, "");
    asts_print(ast->params, indent + AST_INDENT);
    printf("%*s    ret_type: \n", (int)indent, "");
    ast_data_type_print(ast->return_data_type, indent + AST_INDENT);
}

void ast_print(AST* ast, size_t indent)
{
    if(ast == NULL) return;
    else if(ast->type == AST_DATA_TYPE)
        ast_data_type_print((ASTDataType*)ast, indent);
    else if(ast->type == AST_FUNC_PARAM)
        ast_func_param_print((ASTFuncParam*)ast, indent);
    else if(ast->type == AST_FUNC_DECL)
        ast_func_decl_print((ASTFuncDecl*)ast, indent);
}

void ast_data_type_free(ASTDataType* ast)
{
    if(ast == NULL) return;
    free(ast);
}

void ast_func_param_free(ASTFuncParam* ast)
{
    if(ast == NULL) return;
    ast_data_type_free(ast->data_type);
    free(ast);
}

void ast_func_decl_free(ASTFuncDecl* ast)
{
    if(ast == NULL) return;
    asts_free(ast->params);
    ast_data_type_free(ast->return_data_type);
    free(ast);
}

void ast_free(AST* ast)
{
    if(ast == NULL) return;
    else if(ast->type == AST_DATA_TYPE)
        ast_data_type_free((ASTDataType*)ast);
    else if(ast->type == AST_FUNC_PARAM)
        ast_func_param_free((ASTFuncParam*)ast);
    else if(ast->type == AST_FUNC_DECL)
        ast_func_decl_free((ASTFuncDecl*)ast);
}