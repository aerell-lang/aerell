// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/parser.hpp"

namespace aerell
{

Parser::Parser(Lexer& lexer) : lexer(lexer) {}

AST Parser::parse()
{
    AST ast(this->lexer.getFile());

    const Token& token = this->lexer.getToken();
    while(token.getKind() != TokenKind::EOFF)
    {
        if(token.getKind() == TokenKind::INTL) ast.addIntl(token.getOffset(), token.getSize());
        this->lexer.getToken();
    }

    return ast;
}

void Parser::debug()
{
    AST ast = this->parse();
    ast.debug();
}

} // namespace aerell