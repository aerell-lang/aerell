/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/semantic/semantic.h"
#include <aerell/compiler/symbol/symbol_print.h>
#include <aerell/support/utils.h>

namespace aerell
{

Semantic::Semantic(SymbolTable& symbolTable) : symbolTable(&symbolTable) {}

bool Semantic::analysis(const AST::ChildrenWithSource& childrenWithSource)
{
    for(const AST::Ptr& ptr : childrenWithSource.children) stmt(ptr);
    return !this->hasError;
}

void Semantic::stmt(const std::unique_ptr<AST>& ast)
{
    if(auto* funcCtx = dynamic_cast<ASTFunc*>(ast.get())) return func(*funcCtx);
    if(expr(ast).has_value()) return;
}

std::optional<DataType> Semantic::expr(const std::unique_ptr<AST>& ast)
{
    if(auto* funcCallCtx = dynamic_cast<ASTFuncCall*>(ast.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<ASTLiteral*>(ast.get())) return literal(*literalCtx);
    return std::nullopt;
}

void Semantic::func(ASTFunc& ctx)
{
    if(ctx.stmts == std::nullopt) return;
    const auto& blockScope = ctx.symbol->getBlockScope();
    this->symbolTable = blockScope;
    for(const auto& stmt : ctx.stmts.value()) expr(stmt);
    this->symbolTable = blockScope->getParentScope();
}

std::optional<DataType> Semantic::funcCall(ASTFuncCall& ctx)
{
    const auto& indent = ctx.ident;

    auto symbolFunc = this->symbolTable->findFunc(indent->getText(), true);
    if(symbolFunc == nullptr)
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] Undefined function");
        if(!this->hasError) this->hasError = true;
        return std::nullopt;
    }

    std::vector<DataType> args;
    for(const auto& arg : ctx.args)
    {
        auto type = expr(arg);
        if(!type.has_value()) continue;
        args.push_back(type.value());
    }

    if(symbolFunc->getParams().size() != args.size())
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] Invalid number of function arguments");
        if(!this->hasError) this->hasError = true;
        return std::nullopt;
    }

    if(args != symbolFunc->getParams())
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] The function argument is incorrect");
        if(!this->hasError) this->hasError = true;
        return std::nullopt;
    }

    ctx.symbolCalled = symbolFunc;

    return ctx.symbolCalled->getRet();
}

std::optional<DataType> Semantic::literal(ASTLiteral& ctx)
{
    if(ctx.value->type == TokenType::INTL) return DataType::I32;
    if(ctx.value->type == TokenType::STRL) return DataType::STR;
    else
        return std::nullopt;
}

} // namespace aerell