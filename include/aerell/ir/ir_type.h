/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_IR_IR_TYPE_H
#define AERELL_IR_IR_TYPE_H

#include <array>
#include <utility>
#include <vector>

#include "aerell/support/ostream.h"

namespace aerell
{

enum class IRType
{
    I32,
    F32,
    CHR,
    STR
};

using IRTypes = std::vector<IRType>;

constexpr std::array<std::pair<IRType, const char*>, 4> IrTypeNames{{
    {IRType::I32, "I32"},
    {IRType::F32, "F32"},
    {IRType::CHR, "CHR"},
    {IRType::STR, "STR"},
}};

constexpr const char* to_string(IRType t)
{
    for(auto& [k, v] : IrTypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

inline OStream& operator<<(OStream& os, const IRType& obj)
{
    os << to_string(obj);
    return os;
}

inline OStream& operator<<(OStream& os, const IRTypes& obj)
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