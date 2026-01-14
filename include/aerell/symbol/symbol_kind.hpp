// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

namespace aerell
{

enum class SymbolKind : std::uint8_t
{
    NONE,
};

constexpr auto symbolKindStr = std::to_array<std::pair<SymbolKind, std::string_view>>({
    {SymbolKind::NONE, "NONE"},
});

constexpr std::string_view toStr(SymbolKind k)
{
    auto it = std::find_if(symbolKindStr.begin(), symbolKindStr.end(), [k](const auto& p) { return p.first == k; });
    return (it != symbolKindStr.end()) ? it->second : "UNKNOWN";
}

} // namespace aerell