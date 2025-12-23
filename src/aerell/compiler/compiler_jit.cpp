/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#include "aerell/compiler/compiler.h"
#include "aerell/compiler/ir/ir.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Aerell
{

bool Compiler::jit(IR::Modules& modules)
{
    auto module = this->linking(modules);
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

    llvm::orc::ThreadSafeContext ctx{std::move(this->ir.getContext())};

    if(auto error = (*jit)->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), ctx)))
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

} // namespace Aerell