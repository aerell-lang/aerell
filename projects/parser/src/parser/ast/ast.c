#include <stdlib.h>

#include "parser/ast/ast.h"
#include "parser/ast/ast_type.h"

ASTDataType* ast_data_type_create()
{
    ASTDataType* ast = malloc(sizeof(ASTDataType));
    if(!ast) return NULL;
    ast->base.type = AST_DATA_TYPE;
    ast->is_pointer = false;
    ast->value = NULL;
    return ast;
}
ASTFuncParam* ast_func_param_create()
{
    ASTFuncParam* ast = malloc(sizeof(ASTFuncParam));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC_PARAM;
    ast->data_type = NULL;
    ast->id = NULL;
    return ast;
}
ASTFuncDecl* ast_func_decl_create()
{
    ASTFuncDecl* ast = malloc(sizeof(ASTFuncDecl));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC_DECL;
    ast->ret_type = NULL;
    ast->id = NULL;
    ast->is_variadic = false;
    ast->params = NULL;
    return ast;
}
ASTFuncArg* ast_func_arg_create()
{
    ASTFuncArg* ast = malloc(sizeof(ASTFuncArg));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC_ARG;
    ast->value = NULL;
    return ast;
}
ASTFuncCall* ast_func_call_create()
{
    ASTFuncCall* ast = malloc(sizeof(ASTFuncCall));
    if(!ast) return NULL;
    ast->base.type = AST_FUNC_CALL;
    ast->id = NULL;
    ast->args = NULL;
    return ast;
}

void ast_print(AST* ast) {
    if(ast == NULL) return;
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
    free(ast);
}
void ast_func_arg_free(ASTFuncArg* ast)
{
    if(ast == NULL) return;
    free(ast);
}
void ast_func_call_free(ASTFuncCall* ast)
{
    if(ast == NULL) return;
    free(ast);
}
void ast_free(AST* ast)
{
    if(ast == NULL) return;
    switch(ast->type)
    {
    case AST_DATA_TYPE: ast_data_type_free((ASTDataType*)ast); break;
    case AST_FUNC_DECL: ast_func_decl_free((ASTFuncDecl*)ast); break;
    case AST_FUNC_PARAM: ast_func_param_free((ASTFuncParam*)ast); break;
    case AST_FUNC_CALL: ast_func_call_free((ASTFuncCall*)ast); break;
    case AST_FUNC_ARG: ast_func_arg_free((ASTFuncArg*)ast); break;
    }
}