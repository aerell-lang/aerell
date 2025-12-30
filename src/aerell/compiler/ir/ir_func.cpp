/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/ir/ir_func.h"

namespace aerell
{

void IRFunc::print(llvm::raw_ostream& os) const
{
    os << " pub " << this->pub << '\n';
    os << " params " << this->params << '\n';
    os << " vrdic " << this->vrdic << '\n';
    if(this->ret.has_value()) os << " ret " << this->ret.value() << '\n';
    for(const auto& [key, value] : this->blocks) os << ' ' << key << ":\n" << value;
}

void IRFunc::toLlvm(std::string_view ident, IRllvm::Ptr& ptr, IRllvm::Ctx& ctx, llvm::IRBuilder<>& builder) const
{
    // Public
    auto pubLlvm = llvm::Function::InternalLinkage;
    if(this->pub) pubLlvm = llvm::Function::ExternalLinkage;

    // Return
    llvm::Type* retLlvm = builder.getVoidTy();
    if(this->ret.has_value())
    {
        const auto& type = this->ret.value();
        if(type == IRType::I32) retLlvm = builder.getInt32Ty();
        else if(type == IRType::STR)
            retLlvm = builder.getPtrTy();
    }

    // Params
    std::vector<llvm::Type*> paramsLlvm;
    for(auto param : this->params) switch(param)
        {
        case IRType::STR: paramsLlvm.emplace_back(builder.getPtrTy()); break;
        case IRType::I32: paramsLlvm.emplace_back(builder.getInt32Ty()); break;
        }

    // Declare function
    auto fType = llvm::FunctionType::get(retLlvm, paramsLlvm, this->vrdic);
    auto func = llvm::Function::Create(fType, pubLlvm, ident, ptr.get());

    if(this->blocks.empty()) return;

    for(const auto& label : this->blockLabels) this->blocks.at(label).toLlvm(func, label, ptr, ctx, builder);
}

} // namespace aerell