/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/IR/Verifier.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Passes/PassBuilder.h>

#include "aerell/compiler/ir/ir_llvm.h"

namespace aerell
{

bool IRllvm::verify(Ptr& ptr) { return !llvm::verifyModule(*ptr, &errs()); }

void IRllvm::optimize(Ptr& ptr)
{
    llvm::PassBuilder pb;

    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cgsccam;
    llvm::ModuleAnalysisManager mam;

    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgsccam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.crossRegisterProxies(lam, fam, cgsccam, mam);

    llvm::ModulePassManager mpm = pb.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);

    mpm.run(*ptr, mam);
}

IRllvm::Ptr IRllvm::linking(Vec& vec)
{
    if(vec.empty()) return nullptr;

    bool hasError = false;

    auto mainModule = std::move(vec[std::max(0, ((int)vec.size()) - 1)]);

    llvm::Linker linker(*mainModule);

    for(auto& module : vec)
    {
        if(module == nullptr) continue;

        auto sourceFileName = module->getSourceFileName();
        if(linker.linkInModule(std::move(module)))
        {
            errs() << "Failed to link module '" << sourceFileName << "' to '" << mainModule->getSourceFileName()
                   << "'\n";
            hasError = true;
        }
    }

    if(hasError) return nullptr;

    return mainModule;
}

} // namespace aerell