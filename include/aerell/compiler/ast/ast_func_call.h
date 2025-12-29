/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_AST_AST_FUNC_CALL_H
#define AERELL_COMPILER_AST_AST_FUNC_CALL_H

#include <aerell/compiler/ast/ast.h>
#include <aerell/compiler/symbol/symbol_func.h>
#include <aerell/compiler/token/token.h>

namespace aerell
{

class ASTFuncCall : public AST
{
  public:
    ~ASTFuncCall() {};

    const Token* ident = nullptr;
    const SymbolFunc* symbolCalled = nullptr;
    Children args;

    void print(llvm::raw_ostream& os, size_t indent = 0) const override;
};

} // namespace aerell

#endif