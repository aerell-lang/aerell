#pragma once

#include "llvm/IR/Module.h"

namespace aerell
{

class CodeGen
{
  public:
    enum class Type
    {
        ASM,
        OBJ
    };

    static void initialize();
    static bool gen(Type type, const char* name, const std::unique_ptr<llvm::Module>& module);
};

} // namespace aerell