// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/parser.hpp"
#include "aerell/message.hpp"
#include <cassert>

namespace aerell
{

Parser::Parser(Lexer& lexer) : lexer{lexer}, ast{this->lexer.getFile()} {}

std::optional<AST> Parser::parse()
{
    this->reset();

    this->parseRoot();

    if(this->hasError) return std::nullopt;

    return std::move(this->ast);
}

void Parser::reset()
{
    if(this->hasError) this->hasError = false;
    this->ast.reset();
}

#define PARSE_INTL_START_TOKEN_KIND (this->lexer.getToken().getKind() == TokenKind::INTL)
#define PARSE_EXPR_START_TOKEN_KIND PARSE_INTL_START_TOKEN_KIND
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

        Message::print(
            ErrorCode::E0, this->lexer.getFile(),
            {"help: this character isn't supported here, please delete it", this->lexer.getToken().getLexeme(), true});
        this->hasError = true;

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

    // intl
    this->ast.setData1(d1i, this->parseIntl());

    return expri;
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