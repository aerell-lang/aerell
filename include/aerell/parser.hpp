// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/lexer.hpp"

namespace aerell
{

class Parser
{
  public:
    Parser(Lexer& lexer);

    void debug();

  private:
    Lexer& lexer;
};

} // namespace aerell