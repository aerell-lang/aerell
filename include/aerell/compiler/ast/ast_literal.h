/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_AST_AST_LITERAL_H
#define AERELL_COMPILER_AST_AST_LITERAL_H

#include <aerell/compiler/ast/ast.h>
#include <aerell/compiler/token/token.h>

namespace aerell
{

class ASTLiteral : public AST
{
  public:
    ~ASTLiteral() {};

    const Token* value = nullptr;

    void print(llvm::raw_ostream& os, size_t indent = 0) const override;
};

} // namespace aerell

#endif