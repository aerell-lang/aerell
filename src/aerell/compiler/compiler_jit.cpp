#include "aerell/compiler/compiler.h"
#include "aerell/compiler/ir/ir.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace aerell
{

bool Compiler::jit(const char* filePath)
{
    // IR Gen
    auto module = genIR(filePath);
    if(module == nullptr) return false;

    // JIT
    auto jit = llvm::orc::LLJITBuilder().create();
    if(!jit)
    {
        llvm::errs() << jit.takeError() << "\n";
        return false;
    }

#if defined(_WIN32)
    LoadLibraryA("libaerell.dll");
#else
#error "Only win supported"
#endif

    if(auto error = (*jit)->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), std::move(IR::llvmContext))))
    {
        llvm::errs() << error << "\n";
        return false;
    }

    auto entrySym = (*jit)->lookup("_start");
    if(!entrySym)
    {
        llvm::errs() << entrySym.takeError() << "\n";
        return false;
    }

    auto* entry = entrySym->toPtr<void (*)()>();
    entry();

    return true;
}

} // namespace aerell