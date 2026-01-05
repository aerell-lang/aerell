/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/lexer/lexer.h"

namespace aerell
{

static std::pair<TokenType, std::string_view> symbols[] = {
    {TokenType::LPAREN, "("}, {TokenType::RPAREN, ")"},  {TokenType::LBRACE, "{"}, {TokenType::RBRACE, "}"},
    {TokenType::COMMA, ","},  {TokenType::VRDIC, "..."}, {TokenType::ASSIGN, "="},
};

static std::pair<TokenType, std::string_view> keywords[] = {
    {TokenType::I32, "i32"}, {TokenType::I64, "i64"}, {TokenType::F32, "f32"},
    {TokenType::F64, "f64"}, {TokenType::CHR, "chr"}, {TokenType::STR, "str"},
};

Lexer::Lexer(SourceManager& sourceManager, CharReader& charReader)
    : _sourceManager{sourceManager}, _charReader{charReader}
{
}

bool Lexer::lexing(std::string_view source, Results& results)
{
    bool hasError = false;

    std::vector<std::pair<std::string_view, bool>> sources{
        {"std/std", false},
        {source, true},
    };

    for(const auto& source : sources)
    {
        int status = this->_sourceManager.import(source.first, source.second, &errs());
        if(status == 0)
        {
            errs() << '\n';
            if(!hasError) hasError = true;
            continue;
        }
        else if(status == 2)
            continue;

        Results tempResults;
        if(!this->lexing(this->_sourceManager.last(), tempResults))
        {
            if(!hasError) hasError = true;
            continue;
        }

        for(auto& tempResult : tempResults) results.push_back(std::move(tempResult));
    }

    return !hasError;
}

bool Lexer::lexing(const Source& source, Results& results)
{
    bool hasError = false;
    size_t lastImportIndex = 0;
    auto result = this->lexing(source);
    for(const auto& token : result.tokens)
    {
        if(token.type() != TokenType::STRL) break;
        lastImportIndex++;

        std::string message;
        OSStream os(message);
        int status = this->_sourceManager.import(token.lexeme().substr(1, token.lexeme().size() - 2), false, &os);
        if(status == 0)
        {
            message = {"[I] " + message};
            token.print(message);
            if(!hasError) hasError = true;
            continue;
        }
        else if(status == 2)
            continue;

        Results tempResults;
        if(!this->lexing(this->_sourceManager.last(), tempResults))
        {
            if(!hasError) hasError = true;
            continue;
        }

        for(auto& tempResult : tempResults) results.emplace_back(std::move(tempResult));
    }

    result.tokens = {
        std::make_move_iterator(result.tokens.begin() + lastImportIndex), std::make_move_iterator(result.tokens.end())};
    if(result.tokens.size() != 1 && result.tokens[0].type() != TokenType::EOFF) results.emplace_back(result);

    return !hasError;
}

Lexer::Result Lexer::lexing(const Source& source)
{
    this->_source = &source;
    this->_charReader.set(this->_source->content());
    Lexer::Result result{this->_source, {}};

    Token token = this->getToken();
    while(token.type() != TokenType::EOFF)
    {
        result.tokens.emplace_back(std::move(token));
        token = this->getToken();
    }

    result.tokens.emplace_back(std::move(token));

    return result;
}

void Lexer::createToken(TokenType type, size_t offset, size_t size)
{
    this->_token = {this->_source, type, offset, size};
}

void Lexer::createToken(TokenType type, size_t size) { this->createToken(type, this->_charReader.tell(), size); }

// Token
bool Lexer::hasToken() const { return this->_charReader.canAdvance(); }

Token Lexer::getToken()
{
    if(!this->hasToken())
    {
        this->eofHandler();
        return std::move(this->_token);
    }

    while(this->hasToken() && (this->wsHandler() || this->commentHandler()));

    if(this->symbolHandler() || this->keywordHandler() || this->identHandler() || this->fltHandler() ||
       this->intHandler() || this->chrHandler() || this->strHandler())
        return std::move(this->_token);

    this->illegalHandler();
    return std::move(this->_token);
}

bool Lexer::isDigit(char character) const { return std::isdigit(character); }
bool Lexer::isDigit(size_t offset) const { return this->isDigit(this->_charReader.peek(offset)); }

bool Lexer::isIdentStart(char character) const { return character == '_' || std::iswalpha(character); }
bool Lexer::isIdentStart(size_t offset) const { return this->isIdentStart(this->_charReader.peek(offset)); }

bool Lexer::isIdentContinue(char character) const { return character == '_' || std::iswalnum(character); }
bool Lexer::isIdentContinue(size_t offset) const { return this->isIdentContinue(this->_charReader.peek(offset)); }

void Lexer::eofHandler() { this->createToken(TokenType::EOFF, 1); }

void Lexer::illegalHandler()
{
    size_t size = 1;
    this->createToken(TokenType::ILLEGAL, size);
    this->_charReader.advance(size);
}

bool Lexer::wsHandler()
{
    if(std::iswspace(this->_charReader.peek()))
    {
        this->_charReader.advance();
        return true;
    }
    return false;
}

bool Lexer::commentHandler()
{
    // #
    if(!this->_charReader.match('#')) return false;
    this->_charReader.advance();

    // . exclude \n
    while(this->_charReader.canAdvance() && !this->_charReader.match('\n')) this->_charReader.advance();

    return true;
}

bool Lexer::symbolHandler()
{
    for(const auto& [type, symbol] : symbols)
    {
        size_t size = symbol.size();
        if(!this->_charReader.canAdvance(size) || !this->_charReader.match(symbol)) continue;
        this->createToken(type, size);
        this->_charReader.advance(size);
        return true;
    }
    return false;
}

bool Lexer::keywordHandler()
{
    for(const auto& [type, keyword] : keywords)
    {
        size_t size = keyword.size();
        if(!this->_charReader.canAdvance(size) || !this->_charReader.match(keyword) || this->isIdentContinue(size))
            continue;
        this->createToken(type, size);
        this->_charReader.advance(size);
        return true;
    }
    return false;
}

bool Lexer::intHandler()
{
    // [0-9]
    if(!this->isDigit()) return false;

    size_t offset = this->_charReader.tell();
    this->_charReader.advance();

    // [0-9]*
    while(this->_charReader.canAdvance() && this->isDigit()) this->_charReader.advance();

    // Calculate size
    size_t size = this->_charReader.tell() - offset;

    //
    this->createToken(TokenType::INTL, offset, size);

    return true;
}

bool Lexer::fltHandler()
{
    // [0-9]
    if(!this->isDigit()) return false;

    size_t offset = this->_charReader.tell();
    this->_charReader.advance();

    // [0-9]*
    while(this->_charReader.canAdvance() && this->isDigit()) this->_charReader.advance();

    // .
    if(!this->_charReader.match('.'))
    {
        this->_charReader.seek(offset);
        return false;
    }
    this->_charReader.advance();

    // [0-9]
    if(!this->isDigit())
    {
        this->_charReader.seek(offset);
        return false;
    }
    this->_charReader.advance();

    // [0-9]*
    while(this->_charReader.canAdvance() && this->isDigit()) this->_charReader.advance();

    // Calculate size
    size_t size = this->_charReader.tell() - offset;

    //
    this->createToken(TokenType::FLTL, offset, size);

    return true;
}

bool Lexer::chrHandler()
{
    // '
    if(!this->_charReader.match('\'')) return false;
    size_t offset = this->_charReader.tell();
    this->_charReader.advance();

    // Escape
    if(this->_charReader.match('\\'))
    {
        this->_charReader.advance();
        if(this->_charReader.canAdvance()) this->_charReader.advance();
    }
    else
    {
        // .
        char c = this->_charReader.peek();
        if(c == '\'' || c == '\n')
        {
            this->_charReader.seek(offset);
            return false;
        }
        this->_charReader.advance();
    }

    // '
    if(!this->_charReader.match('\''))
    {
        this->_charReader.seek(offset);
        return false;
    }
    this->_charReader.advance();

    // Calculate size
    size_t size = this->_charReader.tell() - offset;

    //
    this->createToken(TokenType::CHRL, offset, size);

    return true;
}

bool Lexer::strHandler()
{
    if(!this->_charReader.match('"')) return false;

    size_t offset = this->_charReader.tell();
    this->_charReader.advance();

    while(this->_charReader.canAdvance())
    {
        char c = this->_charReader.peek();

        if(c == '"' || c == '\n')
        {
            this->_charReader.advance();
            break;
        }

        if(c == '\\') this->_charReader.advance();

        this->_charReader.advance();
    }

    size_t size = this->_charReader.tell() - offset;

    this->createToken(TokenType::STRL, offset, size);

    return true;
}

bool Lexer::identHandler()
{
    if(!this->isIdentStart()) return false;

    size_t offset = this->_charReader.tell();
    this->_charReader.advance();

    while(this->_charReader.canAdvance() && this->isIdentContinue()) this->_charReader.advance();

    size_t size = this->_charReader.tell() - offset;

    this->createToken(TokenType::IDENT, offset, size);

    return true;
}

} // namespace aerell