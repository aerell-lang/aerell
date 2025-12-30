/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/ir/ir_block.h"

namespace aerell
{

void IRBlock::addInstruct(IRInstruct::Ptr ptr) { this->vec.emplace_back(std::move(ptr)); }

void IRBlock::print(llvm::raw_ostream& os) const
{
    for(const IRInstruct::Ptr& ptr : this->vec) os << ' ' << ptr << '\n';
}

void IRBlock::toLlvm(
    llvm::Function* func, std::string_view label, IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    auto* basicBlock = llvm::BasicBlock::Create(*ctx.get(), label, func);
    builder.SetInsertPoint(basicBlock);

    for(const IRInstruct::Ptr& instructPtr : this->vec) instructPtr->toLlvm(ptr, ctx, builder);

    if(func->getReturnType() == builder.getVoidTy()) builder.CreateRetVoid();
}

} // namespace aerell