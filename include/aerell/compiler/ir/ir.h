/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_H
#define AERELL_COMPILER_IR_IR_H

#include <memory>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"

#include <aerell/compiler/ast/ast_func.h>
#include <aerell/compiler/ast/ast_func_call.h>
#include <aerell/compiler/ast/ast_literal.h>
#include <aerell/compiler/symbol/symbol_func.h>

namespace aerell
{

class IR
{
  public:
    using Ptr = std::unique_ptr<llvm::Module>;
    using Vec = std::vector<Ptr>;
    using Ctx = std::unique_ptr<llvm::LLVMContext>;

    struct Unit
    {
        Ctx ctx = nullptr;
        Vec vec;
    };

    bool generating(const AST::Groups& groups, Unit& unit);
    void optimize(Ptr& ptr);
    Ptr linking(Vec& vec);

  private:
    bool hasError = false;

    Ctx* ctx = nullptr;
    llvm::IRBuilder<>* builder = nullptr;

    Ptr defaultModule = nullptr;
    Ptr entryPointModule = nullptr;

    Ptr* module = nullptr;

    bool verify(Ptr& ptr);

    void stmt(const AST::Ptr& ptr);
    llvm::Value* expr(const AST::Ptr& ptr);
    llvm::Function* funcDecl(const Token& ident, const SymbolFunc& ctx);
    void func(ASTFunc& ctx);
    llvm::Value* funcCall(ASTFuncCall& ctx);
    llvm::Value* literal(ASTLiteral& ctx);
};

void print(const IR::Ptr& ptr);

void print(const IR::Vec& vec);

void print(const IR::Unit& unit);

} // namespace aerell

#endif