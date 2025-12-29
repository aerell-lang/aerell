/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SYMBOL_DATA_TYPE_H
#define AERELL_COMPILER_SYMBOL_DATA_TYPE_H

#include <array>

namespace aerell
{

enum DataType
{
    I32,
    STR
};

constexpr std::array<std::pair<DataType, const char*>, 3> TypeNames{{
    {DataType::I32, "I32"},
    {DataType::STR, "STR"},
}};

constexpr const char* to_string(DataType t)
{
    for(auto& [k, v] : TypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

} // namespace aerell

#endif