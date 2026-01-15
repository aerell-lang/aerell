// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

namespace aerell
{

enum class InstKind : std::uint8_t
{
    PRINT
};

constexpr auto instKindStr = std::to_array<std::pair<InstKind, std::string_view>>({
    {InstKind::PRINT, "print"},
});

constexpr std::string_view toStr(InstKind k)
{
    auto it = std::find_if(instKindStr.begin(), instKindStr.end(), [k](const auto& p) { return p.first == k; });
    return (it != instKindStr.end()) ? it->second : "unknown";
}

} // namespace aerell