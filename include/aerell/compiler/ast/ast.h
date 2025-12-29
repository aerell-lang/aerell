/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_AST_AST_H
#define AERELL_COMPILER_AST_AST_H

#include <memory>
#include <vector>

#include <aerell/compiler/symbol/symbol_func.h>
#include <aerell/compiler/symbol/symbol_var.h>
#include <aerell/compiler/token/token.h>

namespace Aerell
{

class AST
{
  public:
    using Ptr = std::unique_ptr<AST>;
    using Children = std::vector<Ptr>;

    struct ChildrenWithSource
    {
        const char* source = nullptr;
        Children children;
    };

    using Groups = std::vector<ChildrenWithSource>;

    virtual ~AST() {};
};

class FuncParam
{
  public:
    const Token* ident = nullptr;
    const Token* type = nullptr;
};

class Func : public AST
{
  public:
    ~Func() {};
    const Token* ident = nullptr;
    std::vector<FuncParam> params;
    const Token* ret = nullptr;
    std::optional<Children> stmts = std::nullopt;
    SymbolFunc* symbol = nullptr;
};

class FuncCall : public AST
{
  public:
    ~FuncCall() {};

    const Token* ident = nullptr;
    const SymbolFunc* symbolCalled = nullptr;
    Children args;
};

class Literal : public AST
{
  public:
    ~Literal() {};

    const Token* value = nullptr;
};

void print(const AST::Ptr& ptr, size_t indent = 0);

void print(const AST::Children& children);

void print(const AST::ChildrenWithSource& childrenWithSource);

void print(const AST::Groups& groups);

} // namespace Aerell

#endif