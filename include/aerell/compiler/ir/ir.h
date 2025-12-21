#pragma once

#include <memory>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"

#include <aerell/compiler/ast/ast.h>
#include <aerell/compiler/symbol/symbol_func.h>

namespace Aerell
{

void print(const std::unique_ptr<llvm::Module>& module);

class IR
{
  public:
    std::unique_ptr<llvm::LLVMContext>& getContext();
    bool gen(const char* name, const std::vector<std::unique_ptr<AST>>& asts, std::unique_ptr<llvm::Module>& module);

  private:
    bool hasError = false;
    std::unique_ptr<llvm::LLVMContext> llvmContext{std::make_unique<llvm::LLVMContext>()};
    llvm::IRBuilder<> llvmBuilder{*llvmContext};
    std::unique_ptr<llvm::Module> moduleTemp{nullptr};

    llvm::Value* expr(const std::unique_ptr<AST>& ast);
    llvm::Function* funcDecl(const Token& ident, SymbolFunc& ctx);
    void func(Func& ctx);
    llvm::Value* funcCall(FuncCall& ctx);
    llvm::Value* literal(Literal& ctx);
};

} // namespace Aerell