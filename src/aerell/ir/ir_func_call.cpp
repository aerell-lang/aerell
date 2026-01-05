/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/ir/ir_func_call.h"

namespace aerell
{

void IRFuncCall::print(OStream& os) const { os << " call " << this->ident << ' ' << this->args; }

llvm::Value* IRFuncCall::toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    llvm::Function* func = ptr->getFunction(this->ident);

    std::vector<llvm::Value*> args;
    for(const auto& arg : this->args) args.emplace_back(arg->toLlvm(ptr, ctx, builder));

    return builder.CreateCall(func, args);
}

} // namespace aerell