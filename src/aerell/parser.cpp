// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/parser.hpp"
#include "aerell/message.hpp"
#include <cassert>

namespace aerell
{

Parser::Parser(Lexer& lexer) : lexer{lexer}, ast{this->lexer.getFile()} {}

AST Parser::parse()
{
    this->ast.reset();

    this->parseRoot();

    return std::move(this->ast);
}

#define PARSE_INTL_START_TOKEN_KIND (this->lexer.getToken().getKind() == TokenKind::INTL)
#define PARSE_LITERAL_START_TOKEN_KIND PARSE_INTL_START_TOKEN_KIND
#define PARSE_ADD_START_TOKEN_KIND PARSE_LITERAL_START_TOKEN_KIND
#define PARSE_EXPR_START_TOKEN_KIND PARSE_ADD_START_TOKEN_KIND
#define PARSE_STMT_START_TOKEN_KIND PARSE_EXPR_START_TOKEN_KIND

void Parser::parseRoot()
{
    this->lexer.forwardToken();

    this->ast.addKind(ASTKind::ROOT);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    std::uint32_t curr = d1i;

    while(this->lexer.getToken().getKind() != TokenKind::EOFF)
    {
        // stmt*
        if(PARSE_STMT_START_TOKEN_KIND)
        {
            std::uint32_t stmti = this->parseStmt();
            if(this->ast.getData1(d1i) == 0) this->ast.setData1(d1i, stmti);
            else
                this->ast.setData2(curr, stmti);
            curr = stmti;
            continue;
        }

        Message::print(this->lexer.getFile(), ErrorCode::E0, this->lexer.getToken().getLexeme());

        this->lexer.forwardToken();
    }
}

std::uint32_t Parser::parseStmt()
{
    assert(PARSE_STMT_START_TOKEN_KIND);
    if(!PARSE_STMT_START_TOKEN_KIND) return 0;

    std::uint32_t stmti = this->ast.addKind(ASTKind::STMT);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    // expr
    this->ast.setData1(d1i, this->parseExpr());

    return stmti;
}

std::uint32_t Parser::parseExpr()
{
    assert(PARSE_EXPR_START_TOKEN_KIND);
    if(!PARSE_EXPR_START_TOKEN_KIND) return 0;

    std::uint32_t expri = this->ast.addKind(ASTKind::EXPR);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    // add
    this->ast.setData1(d1i, this->parseAdd());

    return expri;
}

std::uint32_t Parser::parseAdd()
{
    assert(PARSE_ADD_START_TOKEN_KIND);
    if(!PARSE_ADD_START_TOKEN_KIND) return 0;

    std::uint32_t literali = this->parseLiteral();

    // (+ literal)
    if(this->lexer.getToken().getKind() == TokenKind::PLUS)
    {
        this->lexer.forwardToken();

        std::uint32_t add = this->ast.addKind(ASTKind::ADD);
        this->ast.addData1(literali);
        this->ast.addData2(0);

        std::uint32_t curr = literali;

        std::uint32_t literali = this->parseLiteral();
        this->ast.setData2(curr, literali);
        curr = literali;

        // (+ literal)*
        while(this->lexer.getToken().getKind() == TokenKind::PLUS)
        {
            this->lexer.forwardToken();

            literali = this->parseLiteral();
            this->ast.setData2(curr, literali);
            curr = literali;
        }

        return add;
    }

    return literali;
}

std::uint32_t Parser::parseLiteral()
{
    assert(PARSE_LITERAL_START_TOKEN_KIND);
    if(!PARSE_LITERAL_START_TOKEN_KIND) return 0;

    std::uint32_t literali = this->ast.addKind(ASTKind::LITERAL);
    std::uint32_t d1i = this->ast.addData1(0);
    this->ast.addData2(0);

    // intl
    this->ast.setData1(d1i, this->parseIntl());

    return literali;
}

std::uint32_t Parser::parseIntl()
{
    assert(PARSE_INTL_START_TOKEN_KIND);
    if(!PARSE_INTL_START_TOKEN_KIND) return 0;
    std::uint32_t index =
        this->ast.addIntl(this->lexer.getToken().getLexeme().offset, this->lexer.getToken().getLexeme().size);
    this->lexer.forwardToken();
    return index;
}

} // namespace aerell
