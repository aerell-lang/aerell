/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/ir/ir_i32.h"

namespace aerell
{

IRI32::IRI32(int value) : value(value) {}

void IRI32::print(OStream& os) const { os << this->value; }

llvm::Value* IRI32::toLlvm(
    [[maybe_unused]] IRllvm::Ptr& ptr, [[maybe_unused]] IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    return builder.getInt32(this->value);
}

} // namespace aerell