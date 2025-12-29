/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_TOKEN_TOKEN_TYPE_H
#define AERELL_COMPILER_TOKEN_TOKEN_TYPE_H

#include <array>

namespace aerell
{

enum class TokenType
{
    EOFF,
    ILLEGAL,
    // Keywords
    F,  // f
    PF, // pf
    // Data Types
    STR, // str
    I32, // i32
    // Symbols
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    COMMA,  // ,
    VRDIC,  // ...
    // Literals
    STRL, // " .* exclude("\n) "
    INTL, // [0-9]+
    // Identifier
    IDENT // [a-zA-Z_] [a-zA-Z0-9_]*
};

constexpr std::array<std::pair<TokenType, const char*>, 15> TokenTypeNames{{
    {TokenType::EOFF, "EOF"},
    {TokenType::ILLEGAL, "ILLEGAL"},
    {TokenType::F, "F"},
    {TokenType::PF, "PF"},
    {TokenType::STR, "STR"},
    {TokenType::I32, "I32"},
    {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"},
    {TokenType::LBRACE, "LBRACE"},
    {TokenType::RBRACE, "RBRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::VRDIC, "VRDIC"},
    {TokenType::STRL, "STRL"},
    {TokenType::INTL, "INTL"},
    {TokenType::IDENT, "IDENT"},
}};

constexpr const char* to_string(TokenType t)
{
    for(auto& [k, v] : TokenTypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

} // namespace aerell

#endif