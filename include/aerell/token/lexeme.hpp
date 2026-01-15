// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace aerell
{

struct Lexeme
{
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
};

} // namespace aerell