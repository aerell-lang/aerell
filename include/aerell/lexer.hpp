// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/file.hpp"
#include "aerell/token/token.hpp"

namespace aerell
{

class Lexer
{
  public:
    Lexer(File& file);

    const Token& getToken();

  private:
    File& file;
    Token token;
};

} // namespace aerell