// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/parser.hpp"
#include <cassert>

namespace aerell
{

Parser::Parser(Lexer& lexer) : lexer{lexer}, ast{this->lexer.getFile()} {}

AST Parser::parse()
{
    this->parseRoot();
    return std::move(this->ast);
}

void Parser::parseRoot()
{
    this->lexer.forwardToken();

    this->ast.addKind(ASTKind::ROOT);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    std::uint32_t curr = d1i;

    while(this->lexer.getToken().getKind() == TokenKind::INTL)
    {
        std::uint32_t stmti = this->parseStmt();
        if(this->ast.getData1(d1i) == 0) this->ast.setData1(d1i, stmti);
        else
            this->ast.setData2(curr, stmti);
        curr = stmti;
    }

    assert(this->lexer.getToken().getKind() == TokenKind::EOFF);
    if(this->lexer.getToken().getKind() != TokenKind::EOFF) return;
    this->lexer.forwardToken();
}

std::uint32_t Parser::parseStmt()
{
    assert(this->lexer.getToken().getKind() == TokenKind::INTL);
    if(this->lexer.getToken().getKind() != TokenKind::INTL) return 0;

    std::uint32_t stmti = this->ast.addKind(ASTKind::STMT);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    this->ast.setData1(d1i, this->parseExpr());

    return stmti;
}

std::uint32_t Parser::parseExpr()
{
    assert(this->lexer.getToken().getKind() == TokenKind::INTL);
    if(this->lexer.getToken().getKind() != TokenKind::INTL) return 0;

    std::uint32_t expri = this->ast.addKind(ASTKind::EXPR);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    this->ast.setData1(d1i, this->parseIntl());

    return expri;
}

std::uint32_t Parser::parseIntl()
{
    assert(this->lexer.getToken().getKind() == TokenKind::INTL);
    if(this->lexer.getToken().getKind() != TokenKind::INTL) return 0;
    std::uint32_t index = this->ast.addIntl(this->lexer.getToken().getOffset(), this->lexer.getToken().getSize());
    this->lexer.forwardToken();
    return index;
}

} // namespace aerell