/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_TYPE_H
#define AERELL_COMPILER_IR_IR_TYPE_H

#include <array>
#include <utility>
#include <vector>

#include <llvm/Support/raw_ostream.h>

namespace aerell
{

enum class IRType
{
    STR,
    I32
};

using IRTypes = std::vector<IRType>;

constexpr std::array<std::pair<IRType, const char*>, 2> IrTypeNames{{
    {IRType::STR, "STR"},
    {IRType::I32, "I32"},
}};

constexpr const char* to_string(IRType t)
{
    for(auto& [k, v] : IrTypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const IRType& obj)
{
    os << to_string(obj);
    return os;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const IRTypes& obj)
{
    os << "[ ";
    bool init = true;
    for(const auto& type : obj)
    {
        if(init) init = false;
        else
            os << ", ";
        os << type;
    }
    os << " ]";
    return os;
}

} // namespace aerell

#endif