/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_FUNC_H
#define AERELL_COMPILER_IR_IR_FUNC_H

#include <optional>
#include <string_view>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/ir/ir_type.h"
#include "aerell/compiler/ir/ir_block.h"
#include <aerell/compiler/ir/ir_llvm.h>

namespace aerell
{

class IRFunc
{
  public:
    template <std::convertible_to<IRTypes> A, std::convertible_to<std::optional<IRType>> B>
    IRFunc(bool pub, A&& params, bool vrdic, B&& ret)
        : pub(pub), params(std::forward<A>(params)), vrdic(vrdic), ret(std::forward<B>(ret))
    {
    }

    template <std::convertible_to<std::string> A, std::convertible_to<IRBlock> B>
    IRBlock* addBlock(A&& label, B&& block)
    {
        if(this->blocks.contains(label)) return nullptr;
        auto b = this->blocks.emplace(std::forward<A>(label), std::forward<B>(block));
        this->blockLabels.emplace_back(b.first->first.c_str());
        return &b.first->second;
    }

    void print(llvm::raw_ostream& os) const;

    void toLlvm(std::string_view ident, IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const;

  private:
    bool pub;
    IRTypes params;
    bool vrdic;
    std::optional<IRType> ret;
    std::vector<const char*> blockLabels;
    std::unordered_map<std::string, IRBlock> blocks;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const IRFunc& obj)
{
    obj.print(os);
    return os;
}

} // namespace aerell

#endif