/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_BLOCK_H
#define AERELL_COMPILER_IR_IR_BLOCK_H

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/ir/ir_instruct.h"
#include <aerell/compiler/ir/ir_llvm.h>

namespace aerell
{

class IRBlock
{
  public:
    void addInstruct(IRInstruct::Ptr ptr);

    void print(llvm::raw_ostream& os) const;

    void toLlvm(
        llvm::Function* func, std::string_view label, IRllvm::Ptr& ptr, IRllvm::Ctx& ctx,
        llvm::IRBuilder<>& builder) const;

  private:
    IRInstruct::Vec vec;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const IRBlock& obj)
{
    obj.print(os);
    return os;
}

} // namespace aerell

#endif