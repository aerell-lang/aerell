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
    typedef std::unique_ptr<llvm::LLVMContext> Context;

    Context& getContext();
    bool generating(const char* sourceFileName, const AST::Asts& asts, Module& module);

  private:
    bool hasError = false;
    Context llvmContext{std::make_unique<llvm::LLVMContext>()};
    llvm::IRBuilder<> llvmBuilder{*llvmContext};
    Module moduleTemp{nullptr};

    llvm::Value* expr(const AST::Ast& ast);
    llvm::Function* funcDecl(const Token& ident, SymbolFunc& ctx);
    void func(Func& ctx);
    llvm::Value* funcCall(FuncCall& ctx);
    llvm::Value* literal(Literal& ctx);
};

void print(const IR::Module& module);

} // namespace Aerell