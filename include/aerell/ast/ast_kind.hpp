// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

namespace aerell
{

enum class ASTKind : std::uint8_t
{
    NONE,
    INTL,
};

constexpr auto astKindStr = std::to_array<std::pair<ASTKind, std::string_view>>({
    {ASTKind::NONE, "NONE"},
    {ASTKind::INTL, "INTL"},
});

constexpr std::string_view toStr(ASTKind k)
{
    auto it = std::find_if(astKindStr.begin(), astKindStr.end(), [k](const auto& p) { return p.first == k; });
    return (it != astKindStr.end()) ? it->second : "UNKNOWN";
}

} // namespace aerell