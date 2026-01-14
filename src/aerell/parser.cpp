// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <print>

#include "aerell/parser.hpp"

namespace aerell
{

Parser::Parser(Lexer& lexer) : lexer(lexer) {}

void Parser::debug()
{
    std::println("This message is from parser debug.");
    this->lexer.debug();
}

} // namespace aerell