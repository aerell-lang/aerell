// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

namespace aerell
{

enum class TokenKind : std::uint8_t
{
    EOFF,
    ILLEGAL,
    PLUS,
    INTL
};

constexpr auto tokenKindStr = std::to_array<std::pair<TokenKind, std::string_view>>({
    {TokenKind::EOFF, "EOFF"},
    {TokenKind::ILLEGAL, "ILLEGAL"},
    {TokenKind::PLUS, "PLUS"},
    {TokenKind::INTL, "INTL"},
});

constexpr std::string_view toStr(TokenKind k)
{
    auto it = std::find_if(tokenKindStr.begin(), tokenKindStr.end(), [k](const auto& p) { return p.first == k; });
    return (it != tokenKindStr.end()) ? it->second : "UNKNOWN";
}

} // namespace aerell
