/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <memory>

#include <aerell/compiler/symbol/symbol_table.h>
#include <aerell/compiler/ast/ast.h>

namespace Aerell
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

    void func(Func& ctx);
    std::optional<DataType> funcCall(FuncCall& ctx);
    std::optional<DataType> literal(Literal& ctx);
};

} // namespace Aerell