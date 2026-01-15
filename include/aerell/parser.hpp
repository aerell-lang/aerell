// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>

#include "aerell/lexer.hpp"
#include "aerell/ast/ast.hpp"

namespace aerell
{

class Parser
{
  public:
    Parser(Lexer& lexer);
    std::optional<AST> parse();

  private:
    bool hasError;
    Lexer& lexer;
    AST ast;

    void reset();

    void parseRoot();
    std::uint32_t parseStmt();
    std::uint32_t parseExpr();
    std::uint32_t parseIntl();
};

} // namespace aerell