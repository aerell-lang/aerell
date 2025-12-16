#pragma once

#include <array>

namespace Aerell
{

enum class TokenType
{
    EOFF,
    ILLEGAL,
    // Symbols
    LPAREN, // (
    RPAREN, // )
    COMMA,  // ,
    // Literals
    STRL, // " .* exclude("\n) "
    // Identifier
    IDENT // [a-zA-Z_] [a-zA-Z0-9_]*
};

constexpr std::array<std::pair<TokenType, const char*>, 7> TokenTypeNames{{
    {TokenType::EOFF, "EOF"},
    {TokenType::ILLEGAL, "ILLEGAL"},
    {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::STRL, "STRL"},
    {TokenType::IDENT, "IDENT"},
}};

constexpr const char* to_string(TokenType t)
{
    for(auto& [k, v] : TokenTypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

} // namespace Aerell