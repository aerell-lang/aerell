// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/file.hpp"
#include "aerell/token/token.hpp"

namespace aerell
{

class Lexer
{
  public:
    Lexer(const File& file);

    const File& getFile() const;
    const Token& getToken();

  private:
    const File& file;
    const char* data;
    Token token;
};

} // namespace aerell