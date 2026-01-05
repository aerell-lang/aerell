/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_IR_IR_F32_H
#define AERELL_IR_IR_F32_H

#include "aerell/ir/ir_val.h"

namespace aerell
{

class IRF32 : public IRVal
{
  public:
    IRF32(float value);

    void print(OStream& os) const override;

    llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const override;

  private:
    float value;
};

} // namespace aerell

#endif