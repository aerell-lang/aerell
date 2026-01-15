// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>

namespace aerell
{

enum class ErrorCode : std::uint8_t
{
    E0, // unsupported character
    E1, // integer literal is too large
};

constexpr auto errorCodeStr = std::to_array<std::pair<ErrorCode, std::string_view>>({
    {ErrorCode::E0, "E0"},
    {ErrorCode::E1, "E1"},
});

constexpr std::string_view toStr(ErrorCode k)
{
    auto it = std::find_if(errorCodeStr.begin(), errorCodeStr.end(), [k](const auto& p) { return p.first == k; });
    return (it != errorCodeStr.end()) ? it->second : "UNKNOWN";
}

} // namespace aerell