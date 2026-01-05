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
    : sourceManager{sourceManager}, charReader{charReader}
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
        int status = this->sourceManager.import(source.first, source.second, &errs());
        if(status == 0)
        {
            errs() << '\n';
            if(!hasError) hasError = true;
            continue;
        }
        else if(status == 2)
            continue;

        Results tempResults;
        if(!this->lexing(this->sourceManager.last(), tempResults))
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
        int status = sourceManager.import(token.lexeme().substr(1, token.lexeme().size() - 2), false, &os);
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
        if(!this->lexing(this->sourceManager.last(), tempResults))
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
    this->charReader.set(source.content());
    Lexer::Result result{source, {}};
    this->source = &source;

    bool stop = false;
    while(!stop)
    {
        auto& token = this->getToken();
        if(token.type() == TokenType::EOFF) stop = true;
        result.tokens.push_back(std::move(token));
    }

    return result;
}

bool Lexer::hasToken() const { return this->charReader.canAdvance(); }

Token& Lexer::getToken()
{
    while(this->hasToken())
    {
        if(this->wsHandler() || this->commentHandler()) continue;
        break;
    }

    if(!this->hasToken())
    {
        this->eofHandler();
        return this->token;
    }

    if(this->identHandler() || this->keywordHandler() || this->symbolHandler() || this->intHandler() ||
       this->fltHandler() || this->chrHandler() || this->strHandler())
        return this->token;

    this->illegalHandler();
    return this->token;
}

void Lexer::createToken(TokenType type, size_t offset, size_t size)
{
    this->token = {this->source, type, offset, size};
}

void Lexer::createToken(TokenType type, size_t size) { this->createToken(type, this->charReader.tell(), size); }

bool Lexer::isDigit(char character) const { return std::isdigit(character); }
bool Lexer::isDigit(size_t offset) const { return this->isDigit(this->charReader.peek(offset)); }

bool Lexer::isIdentStart(char character) const { return character == '_' || std::isalpha(character); }
bool Lexer::isIdentStart(size_t offset) const { return this->isIdentStart(this->charReader.peek(offset)); }

bool Lexer::isIdentContinue(char character) const { return character == '_' || std::isalnum(character); }
bool Lexer::isIdentContinue(size_t offset) const { return this->isIdentContinue(this->charReader.peek(offset)); }

void Lexer::eofHandler() { this->createToken(TokenType::EOFF, 1); }

void Lexer::illegalHandler()
{
    size_t size = 1;
    this->createToken(TokenType::ILLEGAL, size);
    this->charReader.advance(size);
}

bool Lexer::wsHandler()
{
    if(std::isspace(this->charReader.peek()))
    {
        this->charReader.advance();
        return true;
    }
    return false;
}

bool Lexer::commentHandler()
{
    // #
    if(!this->charReader.match('#')) return false;
    this->charReader.advance();

    // .
    while(this->charReader.canAdvance())
    {
        // Exclude \n
        if(this->charReader.match('\n')) break;
        this->charReader.advance();
    }

    return true;
}

bool Lexer::symbolHandler()
{
    for(const auto& [type, symbol] : symbols)
    {
        size_t size = symbol.size();
        if(!this->charReader.canAdvance(size) || !this->charReader.match(symbol)) continue;
        this->createToken(type, size);
        this->charReader.advance(size);
        return true;
    }
    return false;
}

bool Lexer::keywordHandler()
{
    for(const auto& [type, keyword] : keywords)
    {
        size_t size = keyword.size();
        if(!this->charReader.canAdvance(size) || !this->charReader.match(keyword) || this->isIdentContinue(size))
            continue;
        this->createToken(type, size);
        this->charReader.advance(size);
        return true;
    }
    return false;
}

bool Lexer::intHandler()
{
    // [0-9]
    if(!this->isDigit()) return false;

    size_t offset = this->charReader.tell();
    this->charReader.advance();

    // [0-9]*
    while(this->charReader.canAdvance() && this->isDigit()) this->charReader.advance();

    // Calculate size
    size_t size = this->charReader.tell() - offset;

    //
    this->createToken(TokenType::INTL, offset, size);

    return true;
}

bool Lexer::fltHandler()
{
    // [0-9]
    if(!this->isDigit()) return false;

    size_t offset = this->charReader.tell();
    this->charReader.advance();

    // [0-9]*
    while(this->charReader.canAdvance() && this->isDigit()) this->charReader.advance();

    // .
    if(!this->charReader.match('.'))
    {
        this->charReader.seek(offset);
        return false;
    }
    this->charReader.advance();

    // [0-9]
    if(!this->isDigit())
    {
        this->charReader.seek(offset);
        return false;
    }
    this->charReader.advance();

    // [0-9]*
    while(this->charReader.canAdvance() && this->isDigit()) this->charReader.advance();

    // Calculate size
    size_t size = this->charReader.tell() - offset;

    //
    this->createToken(TokenType::FLTL, offset, size);

    return true;
}

bool Lexer::chrHandler()
{
    // '
    if(!this->charReader.match('\'')) return false;
    size_t offset = this->charReader.tell();
    this->charReader.advance();

    // Escape
    if(this->charReader.match('\\'))
    {
        this->charReader.advance();
        if(this->charReader.canAdvance()) this->charReader.advance();
    }
    else
    {
        // .
        char c = this->charReader.peek();
        if(c == '\'' || c == '\n')
        {
            this->charReader.seek(offset);
            return false;
        }
        this->charReader.advance();
    }

    // '
    if(!this->charReader.match('\''))
    {
        this->charReader.seek(offset);
        return false;
    }
    this->charReader.advance();

    // Calculate size
    size_t size = this->charReader.tell() - offset;

    //
    this->createToken(TokenType::CHRL, offset, size);

    return true;
}

bool Lexer::strHandler()
{
    if(!this->charReader.match('"')) return false;

    size_t offset = this->charReader.tell();
    this->charReader.advance();

    while(this->charReader.canAdvance())
    {
        char c = this->charReader.peek();

        if(c == '"' || c == '\n')
        {
            this->charReader.advance();
            break;
        }

        if(c == '\\') this->charReader.advance();

        this->charReader.advance();
    }

    size_t size = this->charReader.tell() - offset;

    this->createToken(TokenType::STRL, offset, size);

    return true;
}

bool Lexer::identHandler()
{
    if(!this->isIdentStart()) return false;

    size_t offset = this->charReader.tell();
    this->charReader.advance();

    while(this->charReader.canAdvance())
    {
        if(this->isIdentContinue()) this->charReader.advance();
        else
            break;
    }

    size_t size = this->charReader.tell() - offset;

    this->createToken(TokenType::IDENT, offset, size);

    return true;
}

} // namespace aerell