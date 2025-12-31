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
    F, // function
    P, // public
    // Data Types
    I32, // i32
    F32, // f32
    CHR, // chr
    STR, // str
    // Symbols
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    COMMA,  // ,
    VRDIC,  // ...
    // Literals
    FLTL, // [0-9]+.[0-9]+
    INTL, // [0-9]+
    CHRL, // ' . '
    STRL, // " .* exclude("\n) "
    // Identifier
    IDENT // [a-zA-Z_] [a-zA-Z0-9_]*
};

constexpr std::array<std::pair<TokenType, const char*>, 19> TokenTypeNames{{
    {TokenType::EOFF, "EOF"},      {TokenType::ILLEGAL, "ILLEGAL"}, {TokenType::F, "F"},
    {TokenType::P, "P"},           {TokenType::I32, "I32"},         {TokenType::F32, "F32"},
    {TokenType::CHR, "CHR"},       {TokenType::STR, "STR"},         {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"}, {TokenType::LBRACE, "LBRACE"},   {TokenType::RBRACE, "RBRACE"},
    {TokenType::COMMA, "COMMA"},   {TokenType::VRDIC, "VRDIC"},     {TokenType::FLTL, "FLTL"},
    {TokenType::INTL, "INTL"},     {TokenType::CHRL, "CHRL"},       {TokenType::STRL, "STRL"},
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