#pragma once

#include "llvm/IR/Module.h"

namespace aerell
{

class CodeGen
{
  public:
    static bool obj(const char* name, const std::unique_ptr<llvm::Module>& module);
};

} // namespace aerell