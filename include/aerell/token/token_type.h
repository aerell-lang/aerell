/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_TOKEN_TOKEN_TYPE_H
#define AERELL_TOKEN_TOKEN_TYPE_H

#include <array>

#include "aerell/support/ostream.h"

namespace aerell
{

enum class TokenType
{
    EOFF,
    ILLEGAL,

    I32,
    I64,
    F32,
    F64,
    CHR,
    STR,

    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    COMMA,  // ,
    VRDIC,  // ...

    ASSIGN, // =

    FLTL,
    INTL,
    CHRL,
    STRL,

    IDENT
};

constexpr auto tokenTypeNames = std::to_array<std::pair<TokenType, const char*>>({
    {TokenType::EOFF, "EOF"},
    {TokenType::ILLEGAL, "ILLEGAL"},
    //
    {TokenType::I32, "I32"},
    {TokenType::I64, "I64"},
    {TokenType::F32, "F32"},
    {TokenType::F64, "F64"},
    {TokenType::CHR, "CHR"},
    {TokenType::STR, "STR"},
    //
    {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"},
    {TokenType::LBRACE, "LBRACE"},
    {TokenType::RBRACE, "RBRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::VRDIC, "VRDIC"},
    //
    {TokenType::ASSIGN, "ASSIGN"},
    //
    {TokenType::FLTL, "FLTL"},
    {TokenType::INTL, "INTL"},
    {TokenType::CHRL, "CHRL"},
    {TokenType::STRL, "STRL"},
    //
    {TokenType::IDENT, "IDENT"},
});

constexpr const char* toString(TokenType tokenType)
{
    for(auto& [tokenTypeKey, tokenTypeString] : tokenTypeNames)
        if(tokenTypeKey == tokenType) return tokenTypeString;
    return "UNKNOWN";
}

inline OStream& operator<<(OStream& os, TokenType type)
{
    os << toString(type);
    return os;
}

} // namespace aerell

#endif