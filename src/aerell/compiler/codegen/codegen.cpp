/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <filesystem>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

#include "aerell/compiler/codegen/codegen.h"

namespace aerell
{

bool CodeGen::obj(const char* name, const IRllvm::Ptr& ptr)
{
    auto targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    ptr->setTargetTriple(targetTriple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if(target == nullptr)
    {
        errs() << error << "\n";
        return false;
    }

    llvm::TargetOptions opt;
    llvm::TargetMachine* targetMachine = target->createTargetMachine(
        targetTriple, "generic", "", opt, std::nullopt, std::nullopt, llvm::CodeGenOptLevel::Aggressive);

    ptr->setDataLayout(targetMachine->createDataLayout());

    std::error_code errorCode;
    llvm::raw_fd_ostream dest(
        std::filesystem::path(name).filename().replace_extension("o").string(), errorCode, llvm::sys::fs::OpenFlags());

    if(errorCode)
    {
        errs() << "Could not open file: " << errorCode.message() << "\n";
        return false;
    }

    llvm::legacy::PassManager pass;
    if(targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile))
    {
        errs() << "TargetMachine can't emit a file of this type\n";
        return false;
    }

    pass.run(*ptr);
    dest.flush();

    return true;
}

} // namespace aerell