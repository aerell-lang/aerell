/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_AST_AST_FUNC_H
#define AERELL_COMPILER_AST_AST_FUNC_H

#include <aerell/compiler/ast/ast.h>
#include <aerell/compiler/ast/ast_func_param.h>
#include <aerell/compiler/symbol/symbol_func.h>

namespace aerell
{

class Func : public AST
{
  public:
    ~Func() {};
    const Token* ident = nullptr;
    std::vector<ASTFuncParam> params;
    const Token* ret = nullptr;
    std::optional<Children> stmts = std::nullopt;
    SymbolFunc* symbol = nullptr;

    void print(llvm::raw_ostream& os, size_t indent = 0) const override;
};

} // namespace aerell

#endif