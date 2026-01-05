/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_IR_IR_FUNC_CALL_H
#define AERELL_IR_IR_FUNC_CALL_H

#include "aerell/support/ostream.h"
#include "aerell/ir/ir_instruct.h"
#include "aerell/ir/ir_val.h"

namespace aerell
{

class IRFuncCall : public IRInstruct
{
  public:
    template <std::convertible_to<std::string> A, std::convertible_to<IRVal::Vec> B>
    IRFuncCall(A&& ident, B&& args) : ident(std::forward<A>(ident)), args(std::forward<B>(args))
    {
    }

    void print(OStream& os) const override;

    llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const override;

  private:
    std::string ident;
    IRVal::Vec args;
};

} // namespace aerell

#endif