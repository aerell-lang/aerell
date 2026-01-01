/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_CHR_H
#define AERELL_COMPILER_IR_IR_CHR_H

#include <aerell/compiler/ir/ir_val.h>

namespace aerell
{

class IRChr : public IRVal
{
  public:
    IRChr(char value);

    void print(OStream& os) const override;

    llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const override;

  private:
    char value;
};

} // namespace aerell

#endif