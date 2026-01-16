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
    AST ast;

    void parseRoot();
    std::uint32_t parseStmt();
    std::uint32_t parseExpr();
    std::uint32_t parseAdd();
    std::uint32_t parseLiteral();
    std::uint32_t parseIntl();
};

} // namespace aerell
