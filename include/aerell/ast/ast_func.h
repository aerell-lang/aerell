/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_AST_AST_FUNC_H
#define AERELL_AST_AST_FUNC_H

#include "aerell/support/ostream.h"
#include "aerell/ast/ast.h"
#include "aerell/ast/ast_func_param.h"
#include "aerell/symbol/symbol_func.h"

namespace aerell
{

class ASTFunc : public AST
{
  public:
    ~ASTFunc() {};
    const Token* ident = nullptr;
    std::vector<ASTFuncParam> params;
    const Token* ret = nullptr;
    std::optional<List> stmts = std::nullopt;
    SymbolFunc* symbol = nullptr;

    void print(OStream& os, size_t indent = 0) const override;
};

} // namespace aerell

#endif