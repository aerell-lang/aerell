/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <aerell/compiler/ir/ir_f32.h>

namespace aerell
{

IRF32::IRF32(float value) : value(value) {}

void IRF32::print(OStream& os) const { os << this->value; }

llvm::Value* IRF32::toLlvm(
    [[maybe_unused]] IRllvm::Ptr& ptr, [[maybe_unused]] IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    return llvm::ConstantFP::get(builder.getFloatTy(), this->value);
}

} // namespace aerell