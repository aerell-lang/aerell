#include <cwctype>
#include <string_view>

#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/lexer/lexer.h"

namespace aerell
{

const std::pair<std::string, TokenType> Lexer::symbols[] = {
    {"(", TokenType::LPAREN},
    {")", TokenType::RPAREN},
    {",", TokenType::COMMA},
};

std::string Lexer::content;
size_t Lexer::pos = 0;

std::vector<Token> Lexer::tokens;

std::vector<Token> Lexer::gen(const std::string& input)
{
    content = input;

    while(pos < content.size())
    {
        if(isWS()) continue;
        if(isComment()) continue;
        if(isSymbl()) continue;
        if(isStr()) continue;
        if(isIdent()) continue;
        tokens.push_back({.type = TokenType::ILLEGAL, .content = {content[pos]}});
        pos++;
    }

    return std::move(tokens);
}

bool Lexer::isWS()
{
    if(std::iswspace(content[pos]))
    {
        pos++;
        return true;
    }
    return false;
}

bool Lexer::isComment()
{
    if(content[pos] != '#') return false;
    pos++;
    while(pos < content.size() && content[pos] != '\n') pos++;
    return true;
}

bool Lexer::isSymbl()
{
    for(const auto& symbol : symbols)
    {
        const auto& symbolStr = symbol.first;
        size_t size = symbolStr.size();

        if(pos + size > content.size()) continue;

        if(std::string_view(content.data() + pos, size) != symbolStr) continue;

        pos += size;

        tokens.push_back({.type = symbol.second, .content = symbol.first});

        return true;
    }
    return false;
}

bool Lexer::isStr()
{
    if(content[pos] != '"') return false;

    size_t start = pos;
    pos++;

    while(pos < content.size() && content[pos] != '"' && content[pos] != '\n') pos++;

    pos++;
    size_t end = pos;

    tokens.push_back({.type = TokenType::STRL, .content = {content.data() + start, end - start}});

    return true;
}

bool Lexer::isIdent()
{
    if(!std::iswalnum(content[pos]) && content[pos] != '_') return false;

    size_t start = pos;
    pos++;

    while(pos < content.size() && (std::iswalnum(content[pos]) || content[pos] == '_')) pos++;

    size_t end = pos;

    tokens.push_back({.type = TokenType::IDENT, .content = {content.data() + start, end - start}});

    return true;
}

} // namespace aerell