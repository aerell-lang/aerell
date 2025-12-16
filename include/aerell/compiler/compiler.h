#pragma once

#include <aerell/compiler/source/source_manager.h>
#include <llvm/IR/Module.h>
#include <memory>

namespace Aerell
{

class Compiler
{
  public:
    static bool jit(const char* filePath);
    static bool compile(const char* filePath);

  private:
    static SourceManager sourceManager;

    static std::unique_ptr<llvm::Module> genIR(Source* source);
    static std::unique_ptr<llvm::Module> genIR(const char* filePath);
};

} // namespace Aerell