/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <aerell/compiler/ir/ir_str.h>

namespace aerell
{

IRStr::IRStr(std::string&& value) : value(std::move(value)) {}

void IRStr::print(OStream& os) const { os << '"' << this->value << '"'; }

llvm::Value* IRStr::toLlvm(
    [[maybe_unused]] IRllvm::Ptr& ptr, [[maybe_unused]] IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    return builder.CreateGlobalString(this->value);
}

} // namespace aerell