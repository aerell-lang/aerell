/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_TOKEN_TOKEN_H
#define AERELL_TOKEN_TOKEN_H

#include <vector>

#include "aerell/support/ostream.h"
#include "aerell/token/token_type.h"
#include <aerell/source/source.h>

namespace aerell
{

class Token
{
  public:
    using List = std::vector<Token>;

    Token();
    Token(const Source* source, TokenType type, size_t offset, size_t size);

    TokenType type() const;
    std::string_view lexeme() const;

    void print(std::string_view message) const;
    void print(OStream& os) const;

  private:
    const Source* _source;
    TokenType _type;
    size_t _offset;
    size_t _size;
};

inline OStream& operator<<(OStream& os, const Token& token)
{
    token.print(os);
    return os;
}

inline OStream& operator<<(OStream& os, const Token::List& list)
{
    for(const auto& token : list) os << token;
    return os;
}

} // namespace aerell

#endif