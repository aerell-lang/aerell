/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_IR_IR_STR_H
#define AERELL_IR_IR_STR_H

#include <string>

#include "aerell/ir/ir_val.h"

namespace aerell
{

class IRStr : public IRVal
{
  public:
    IRStr(std::string&& value);

    void print(OStream& os) const override;

    llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const override;

  private:
    std::string value;
};

} // namespace aerell

#endif