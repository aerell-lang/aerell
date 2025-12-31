/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_LLVM_H
#define AERELL_COMPILER_IR_IR_LLVM_H

#include <llvm/IR/Module.h>

#include <aerell/support/ostream.h>

namespace aerell
{

class IRllvm
{
  public:
    using Ptr = std::unique_ptr<llvm::Module>;
    using Vec = std::vector<Ptr>;
    using Ctx = std::unique_ptr<llvm::LLVMContext>;

    struct Unit
    {
        Ctx ctx = nullptr;
        Vec vec;
    };

    static bool verify(Ptr& ptr);
    static void optimize(Ptr& ptr);
    static Ptr linking(Vec& vec);

  private:
    IRllvm() {}
};

inline OStream& operator<<(OStream& os, const IRllvm::Ptr& obj)
{
    obj->print(outs(), nullptr);
    return os;
}

inline OStream& operator<<(OStream& os, const IRllvm::Vec& obj)
{
    for(const auto& ptr : obj) os << "\n```\n" << ptr << "\n```\n";
    return os;
}

inline OStream& operator<<(OStream& os, const IRllvm::Unit& obj)
{
    os << obj.vec;
    return os;
}

} // namespace aerell

#endif