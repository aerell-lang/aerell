// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>

#include "aerell/file.hpp"
#include "aerell/token/token_kind.hpp"
#include "aerell/token/lexeme.hpp"

namespace aerell
{

class Token
{
  public:
    Token(const File& file);

    TokenKind getKind() const;

    void setKind(TokenKind kind);
    void setOffset(std::uint32_t offset);
    void setSize(std::uint32_t size);

    std::string toStr() const;

    const Lexeme& getLexeme() const;

  private:
    const File& file;
    TokenKind kind;
    Lexeme lexeme;
};

} // namespace aerell