// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/lexer.hpp"
#include "aerell/ast/ast.hpp"

namespace aerell
{

class Parser
{
  public:
    Parser(Lexer& lexer);
    AST parse();

  private:
    Lexer& lexer;
};

} // namespace aerell