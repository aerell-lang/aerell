/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_MOD_H
#define AERELL_COMPILER_IR_IR_MOD_H

#include <unordered_map>
#include <vector>

#include <llvm/IR/IRBuilder.h>

#include "aerell/compiler/ir/ir_func.h"
#include "aerell/compiler/ir/ir_llvm.h"

namespace aerell
{

class IRMod
{
  public:
    using Ptr = std::unique_ptr<IRMod>;
    using Vec = std::vector<Ptr>;

    template <std::convertible_to<std::string> T> explicit IRMod(T&& name) : name(std::forward<T>(name)) {}

    template <std::convertible_to<std::string> T> void setName(T&& newName) { name = std::forward<T>(newName); }

    template <std::convertible_to<std::string> A, std::convertible_to<IRFunc> B> IRFunc* addFunc(A&& ident, B&& func)
    {
        if(this->funcs.contains(ident)) return nullptr;
        auto f = this->funcs.emplace(std::forward<A>(ident), std::forward<B>(func));
        this->funcIdents.emplace_back(f.first->first.c_str());
        return &f.first->second;
    }

    IRFunc* getFunc(const std::string& ident);

    void print(OStream& os) const;

    IRllvm::Ptr toLlvm(IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const;

  private:
    std::string name;
    std::vector<const char*> funcIdents;
    std::unordered_map<std::string, IRFunc> funcs;
};

inline OStream& operator<<(OStream& os, const IRMod& obj)
{
    obj.print(os);
    return os;
}

inline OStream& operator<<(OStream& os, const IRMod::Ptr& obj)
{
    os << (*obj);
    return os;
}

inline OStream& operator<<(OStream& os, const IRMod::Vec& obj)
{

    for(const IRMod::Ptr& ptr : obj) os << ptr;
    return os;
}

} // namespace aerell

#endif