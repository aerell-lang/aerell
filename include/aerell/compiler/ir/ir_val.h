/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_VAL_H
#define AERELL_COMPILER_IR_IR_VAL_H

#include <memory>
#include <vector>

#include <llvm/IR/IRBuilder.h>

#include <aerell/compiler/ir/ir_llvm.h>

namespace aerell
{

class IRVal
{
  public:
    using Ptr = std::unique_ptr<IRVal>;
    using Vec = std::vector<Ptr>;

    virtual ~IRVal() {}

    virtual void print(OStream& os) const = 0;

    virtual llvm::Value* toLlvm(IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const = 0;
};

inline OStream& operator<<(OStream& os, const IRVal& obj)
{
    obj.print(os);
    return os;
}

inline OStream& operator<<(OStream& os, const IRVal::Ptr& obj)
{
    os << (*obj);
    return os;
}

inline OStream& operator<<(OStream& os, const IRVal::Vec& obj)
{
    os << "[ ";
    bool init = true;
    for(const auto& ptr : obj)
    {
        if(init) init = false;
        else
            os << ", ";
        os << ptr;
    }
    os << " ]";
    return os;
}

} // namespace aerell

#endif