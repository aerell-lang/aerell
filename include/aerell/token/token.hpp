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
    Token(File& file);

    std::size_t getOffset() const;
    void setOffset(std::size_t offset);

    std::size_t getSize() const;
    void setSize(std::size_t size);

  private:
    File& file;
    TokenKind kind;
    std::size_t offset;
    std::size_t size;
};

} // namespace aerell