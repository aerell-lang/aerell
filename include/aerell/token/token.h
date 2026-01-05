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

namespace aerell
{

class Token
{
  public:
    using List = std::vector<Token>;

    Token();
    Token(TokenType type, size_t offset, size_t size);

    TokenType getType() const;
    size_t getOffset() const;
    size_t getSize() const;

    void print(OStream& os) const;

  private:
    TokenType type;
    size_t offset;
    size_t size;
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