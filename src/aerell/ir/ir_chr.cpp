/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/ir/ir_chr.h"

namespace aerell
{

IRChr::IRChr(char value) : value(value) {}

void IRChr::print(OStream& os) const { os << '"' << this->value << '"'; }

llvm::Value* IRChr::toLlvm(
    [[maybe_unused]] IRllvm::Ptr& ptr, [[maybe_unused]] IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    return builder.getInt8(this->value);
}

} // namespace aerell