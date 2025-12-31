/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/IR/LLVMContext.h>
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#include <aerell/support/ostream.h>
#include "aerell/compiler/compiler.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace aerell
{

bool Compiler::jit(IRllvm::Unit& unit)
{
    if(unit.vec.empty()) return false;

    // JIT
    auto jit = llvm::orc::LLJITBuilder().create();
    if(!jit)
    {
        errs() << jit.takeError() << "\n";
        return false;
    }

#if defined(_WIN32)
    LoadLibraryA("libaerell.dll");
#else
#error "Only win supported"
#endif

    llvm::orc::ThreadSafeContext ctx{std::move(unit.ctx)};

    for(IRllvm::Ptr& ptr : unit.vec)
        if(auto error = (*jit)->addIRModule(llvm::orc::ThreadSafeModule(std::move(ptr), ctx)))
        {
            errs() << error << "\n";
            return false;
        }

    auto entrySym = (*jit)->lookup("_start");
    if(!entrySym)
    {
        errs() << entrySym.takeError() << "\n";
        return false;
    }

    auto* entry = entrySym->toPtr<void (*)()>();
    entry();

    return true;
}

} // namespace aerell