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
    Lexer(File& file);

    const Token& getToken();

    void debug();

  private:
    File& file;
    const char* data;
    Token token;
};

} // namespace aerell