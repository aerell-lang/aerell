/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <memory>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"

#include <aerell/compiler/ast/ast.h>
#include <aerell/compiler/symbol/symbol_func.h>

namespace Aerell
{

class IR
{
  public:
    typedef std::unique_ptr<llvm::Module> Module;
    typedef std::vector<std::unique_ptr<llvm::Module>> Modules;
    typedef std::unique_ptr<llvm::LLVMContext> Context;

    Context& getContext();
    bool generating(const char* sourceFileName, const AST::Asts& asts, Module& module);
    Module getStartModule();
    bool linking(Module& module, Module& moduleDest);
    Module linking(Modules& modules);
    void optimize(Module& module);

  private:
    bool hasError = false;
    Context llvmContext{std::make_unique<llvm::LLVMContext>()};
    llvm::IRBuilder<> llvmBuilder{*llvmContext};
    Module moduleTemp{nullptr};
    Module moduleStart{nullptr};
    llvm::BasicBlock* startFuncEntry{nullptr};

    bool verify(Module& module);

    void stmt(const AST::Ast& ast, const Module& module);
    llvm::Value* expr(const AST::Ast& ast, const Module& module);
    llvm::Function* funcDecl(const Token& ident, SymbolFunc& ctx, const Module& module);
    void func(Func& ctx);
    llvm::Value* funcCall(FuncCall& ctx, const Module& module);
    llvm::Value* literal(Literal& ctx);
};

void print(const IR::Module& module);

} // namespace Aerell