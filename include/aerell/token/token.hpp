// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>

#include "aerell/token/token_kind.hpp"

#include "aerell/file.hpp"

namespace aerell
{

class Token
{
  public:
    Token(const File& file);

    TokenKind getKind() const;
    void setKind(TokenKind kind);

    std::uint32_t getOffset() const;
    void setOffset(std::uint32_t offset);

    std::uint32_t getSize() const;
    void setSize(std::uint32_t size);

  private:
    const File& file;
    TokenKind kind;
    std::uint32_t offset;
    std::uint32_t size;
};

} // namespace aerell