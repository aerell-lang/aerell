// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

namespace aerell
{

enum class Type : std::uint8_t
{
    I1,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
};

constexpr auto typeStr = std::to_array<std::pair<Type, std::string_view>>({
    {Type::I1, "I1"},
    {Type::I8, "I8"},
    {Type::I16, "I16"},
    {Type::I32, "I32"},
    {Type::I64, "I64"},
    {Type::U8, "U8"},
    {Type::U16, "U16"},
    {Type::U32, "U32"},
    {Type::U64, "U64"},
});

constexpr std::string_view toStr(Type t)
{
    auto it = std::find_if(typeStr.begin(), typeStr.end(), [t](const auto& p) { return p.first == t; });
    return (it != typeStr.end()) ? it->second : "UNKNOWN";
}

} // namespace aerell