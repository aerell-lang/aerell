/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_STR_H
#define AERELL_COMPILER_IR_IR_STR_H

#include <aerell/compiler/ir/ir_val.h>

#include <string>

namespace aerell
{

class IRStr : public IRVal
{
  public:
    IRStr(std::string&& value);

    void print(llvm::raw_ostream& os) const override;

    llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const override;

  private:
    std::string value;
};

} // namespace aerell

#endif