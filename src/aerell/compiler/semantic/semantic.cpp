/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/semantic/semantic.h"
#include <aerell/support/utils.h>

namespace Aerell
{

Semantic::Semantic(SymbolTable& symbolTable) : symbolTable(&symbolTable) {}

bool Semantic::analysis(const AST::Asts& asts)
{
    for(const auto& ast : asts) stmt(ast);
    return !this->hasError;
}

void Semantic::stmt(const std::unique_ptr<AST>& ast)
{
    if(auto* funcCtx = dynamic_cast<Func*>(ast.get())) return func(*funcCtx);
    if(expr(ast).has_value()) return;
}

std::optional<Type> Semantic::expr(const std::unique_ptr<AST>& ast)
{
    if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) return literal(*literalCtx);
    return std::nullopt;
}

void Semantic::func(Func& ctx)
{
    if(ctx.stmts == std::nullopt) return;
    for(const auto& stmt : ctx.stmts.value()) expr(stmt);
}

std::optional<Type> Semantic::funcCall(FuncCall& ctx)
{
    const auto& indent = ctx.ident;

    auto symbolFunc = this->symbolTable->findFunc(indent->getText());
    if(symbolFunc == nullptr)
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] Undefined function");
        if(!this->hasError) this->hasError = true;
        return std::nullopt;
    }

    std::vector<Type> args;
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

Type Semantic::literal(Literal& ctx)
{
    if(ctx.value->type == TokenType::INTL) return Type::I32;
    if(ctx.value->type == TokenType::STRL) return Type::STR;
    else
        return Type::VOID;
}

} // namespace Aerell