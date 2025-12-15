#pragma once

#include <llvm/IR/Module.h>
#include <memory>
#include <optional>
#include <string>

namespace aerell
{

class Compiler
{
  public:
    static bool jit(const char* filePath);
    static bool compile(const char* filePath);

  private:
    static std::optional<std::string> read(const char* filePath);
    static std::unique_ptr<llvm::Module> genIR(const std::string& fileContent);
    static std::unique_ptr<llvm::Module> genIR(const char* filePath);
};

} // namespace aerell