/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <array>

namespace Aerell
{

enum class SymbolType
{
    VAR,
    FUNC
};

constexpr std::array<std::pair<SymbolType, const char*>, 2> SymbolTypeNames{{
    {SymbolType::VAR, "VAR"},
    {SymbolType::FUNC, "FUNC"},
}};

constexpr const char* to_string(SymbolType t)
{
    for(auto& [k, v] : SymbolTypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

} // namespace Aerell