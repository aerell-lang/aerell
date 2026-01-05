/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_SEMANTIC_SEMANTIC_H
#define AERELL_SEMANTIC_SEMANTIC_H

#include <aerell/parser/parser.h>
#include <memory>

#include "aerell/symbol/symbol_table.h"
#include "aerell/ast/ast_func.h"
#include "aerell/ast/ast_func_call.h"
#include "aerell/ast/ast_literal.h"

namespace aerell
{

class Semantic
{
  public:
    Semantic(SymbolTable& symbolTable);
    bool analysis(const Parser::Result& result);
    bool analysis(const Parser::Results& results);

  private:
    bool hasError = false;
    const SymbolTable* symbolTable;

    void stmt(const std::unique_ptr<AST>& ast);
    std::optional<IRType> expr(const std::unique_ptr<AST>& ast);

    void func(ASTFunc& ctx);
    std::optional<IRType> funcCall(ASTFuncCall& ctx);
    std::optional<IRType> literal(ASTLiteral& ctx);
};

} // namespace aerell

#endif