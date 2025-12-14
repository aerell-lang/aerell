#pragma once

#include "llvm/IR/Module.h"

namespace aerell
{

class CodeGen
{
  public:
    static void initialize();
    static bool object(const std::unique_ptr<llvm::Module>& module, const char* filename);
};

} // namespace aerell