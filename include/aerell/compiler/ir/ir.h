#pragma once

#include <memory>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"

#include <aerell/compiler/ast/ast.h>

namespace Aerell
{

void print(const std::unique_ptr<llvm::Module>& module);

class IR
{
  public:
    static std::unique_ptr<llvm::LLVMContext> llvmContext;

    static bool gen(const std::vector<std::unique_ptr<AST>>& asts, std::unique_ptr<llvm::Module>& module);

  private:
    static llvm::IRBuilder<> llvmBuilder;

    static std::unique_ptr<llvm::Module> moduleTemp;

    static llvm::Value* funcCall(FuncCall& ctx);

    static llvm::Value* literal(Literal& ctx);
};

} // namespace Aerell