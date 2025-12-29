/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SEMANTIC_SEMANTIC_H
#define AERELL_COMPILER_SEMANTIC_SEMANTIC_H

#include <memory>

#include <aerell/compiler/symbol/symbol_table.h>
#include <aerell/compiler/ast/ast_func.h>
#include <aerell/compiler/ast/ast_func_call.h>
#include <aerell/compiler/ast/ast_literal.h>

namespace aerell
{

class Semantic
{
  public:
    Semantic(SymbolTable& symbolTable);
    bool analysis(const AST::ChildrenWithSource& childrenWithSource);

  private:
    bool hasError = false;
    const SymbolTable* symbolTable;

    void stmt(const std::unique_ptr<AST>& ast);
    std::optional<DataType> expr(const std::unique_ptr<AST>& ast);

    void func(ASTFunc& ctx);
    std::optional<DataType> funcCall(ASTFuncCall& ctx);
    std::optional<DataType> literal(ASTLiteral& ctx);
};

} // namespace aerell

#endif