#pragma once

#include <memory>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <aerell/compiler/ast/ast.h>

namespace aerell
{

void print(const std::unique_ptr<llvm::Module>& module);

class IR
{
  public:
    static std::unique_ptr<llvm::LLVMContext> llvmContext;

    static bool verify(const std::unique_ptr<llvm::Module>& module, llvm::raw_ostream* os);

    static void optimize(const std::unique_ptr<llvm::Module>& module);

    static std::unique_ptr<llvm::Module> gen(const std::vector<std::unique_ptr<AST>>& asts);

  private:
    static llvm::IRBuilder<> llvmBuilder;

    static std::unique_ptr<llvm::Module> module;

    static llvm::Value* funcCall(FuncCall& ctx);

    static llvm::Value* literal(Literal& ctx);
};

} // namespace aerell