/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_LEXER_LEXER_H
#define AERELL_LEXER_LEXER_H

#include <vector>

#include <aerell/source/source_manager.h>
#include <aerell/reader/char_reader.h>
#include <aerell/token/token.h>

namespace aerell
{

class Lexer
{
  public:
    struct Result
    {
        const Source& source;
        Token::List tokens;
    };

    using Results = std::vector<Result>;

    Lexer(SourceManager& sourceManager, CharReader& charReader);

    bool lexing(std::string_view source, Results& results);

  private:
    SourceManager& sourceManager;
    CharReader& charReader;

    Token token;

    // Lexing
    bool lexing(const Source& source, Results& results);
    Result lexing(const Source& source);

    // Token
    bool hasToken() const;
    Token& getToken();

    void createToken(TokenType type, size_t offset, size_t size);
    void createToken(TokenType type, size_t size);

    // Fragment
    bool isDigit(char character) const;
    bool isDigit(size_t offset = 0) const;
    bool isIdentStart(char character) const;
    bool isIdentStart(size_t offset = 0) const;
    bool isIdentContinue(char character) const;
    bool isIdentContinue(size_t offset = 0) const;

    // Handler
    void eofHandler();
    void illegalHandler();
    bool wsHandler();
    bool commentHandler();
    bool symbolHandler();
    bool keywordHandler();
    bool intHandler();
    bool fltHandler();
    bool chrHandler();
    bool strHandler();
    bool identHandler();
};

inline OStream& operator<<(OStream& os, const Lexer::Result& result)
{
    size_t maxWidth = 0;

    for(const auto& token : result.tokens)
    {
        const char* t = toString(token.getType());
        size_t width = std::strlen(t);
        maxWidth = std::max(maxWidth, width);
    }

    outs() << '\n' << result.source.path().generic_string() << ":\n";

    for(const auto& token : result.tokens)
    {
        const char* t = toString(token.getType());
        size_t width = std::strlen(t);

        if(width < maxWidth)
        {
            outs() << std::string(maxWidth - width, ' ');
        }

        outs() << t << " " << result.source.content(token) << '\n';
    }
    return os;
}

inline OStream& operator<<(OStream& os, const Lexer::Results& results)
{
    for(const auto& result : results) os << result;
    return os;
}

} // namespace aerell

#endif