/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_LEXER_LEXER_H
#define AERELL_COMPILER_LEXER_LEXER_H

#include <utility>

#include "aerell/compiler/token/token.h"
#include <aerell/compiler/source/source.h>

namespace aerell
{

class Lexer
{
  public:
    Token::Vec lexing(Source* source);

  private:
    static const std::pair<std::string, TokenType> symbols[];
    static const std::pair<std::string, TokenType> keywords[];

    Source* sourceRef = nullptr;
    size_t pos = 0;
    Token::Vec tokens;

    bool isWS();
    bool isComment();
    bool isSymbols();
    bool isKeywords();
    bool isInt();
    bool isFlt();
    bool isChr();
    bool isStr();
    bool isIdent();
};

} // namespace aerell

#endif