/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <memory>

#include <llvm/IR/Module.h>

#include "aerell/compiler/ir/ir_mod.h"
#include <aerell/compiler/ir/ir_func.h>

namespace aerell
{

IRFunc* IRMod::getFunc(const std::string& ident)
{
    if(this->funcs.contains(ident)) return &this->funcs.at(ident);
    return nullptr;
}

void IRMod::print(llvm::raw_ostream& os) const
{
    os << "module_name: " << this->name << '\n';
    for(const auto& ident : this->funcIdents) os << ident << ":\n" << this->funcs.at(ident) << '\n';
}

IRllvm::Ptr IRMod::toLlvm(IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    IRllvm::Ptr mod = std::make_unique<llvm::Module>(this->name, *ctx);

    for(const auto& ident : this->funcIdents) this->funcs.at(ident).toLlvm(ident, mod, ctx, builder);

    if(!IRllvm::verify(mod)) return nullptr;
    return mod;
}

} // namespace aerell