#include <cwctype>
#include <string_view>

#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/lexer/lexer.h"

namespace Aerell
{

const std::pair<std::string, TokenType> Lexer::symbols[] = {{"(", TokenType::LPAREN}, {")", TokenType::RPAREN},
                                                            {"{", TokenType::LBRACE}, {"}", TokenType::RBRACE},
                                                            {",", TokenType::COMMA},  {"...", TokenType::VRDIC}};

const std::pair<std::string, TokenType> Lexer::keywords[] = {
    {"f", TokenType::F},
    {"pf", TokenType::PF},
    {"str", TokenType::STR},
    {"i32", TokenType::I32},
};

std::vector<Token> Lexer::gen(Source* source)
{
    sourceRef = source;
    pos = 0;

    while(pos < sourceRef->getContent().size())
    {
        if(isWS()) continue;
        if(isComment()) continue;
        if(isSymbols()) continue;
        if(isKeywords()) continue;
        if(isInt()) continue;
        if(isStr()) continue;
        if(isIdent()) continue;
        tokens.push_back({.type = TokenType::ILLEGAL, .source = source, .offset = pos, .size = 1});
        pos++;
    }

    tokens.push_back({.type = TokenType::EOFF, .source = source, .offset = pos, .size = 1});

    return std::move(tokens);
}

bool Lexer::isWS()
{
    if(std::iswspace(sourceRef->getContent()[pos]))
    {
        pos++;
        return true;
    }
    return false;
}

bool Lexer::isComment()
{
    if(sourceRef->getContent()[pos] != '#') return false;
    pos++;
    while(pos < sourceRef->getContent().size() && sourceRef->getContent()[pos] != '\n') pos++;
    return true;
}

bool Lexer::isSymbols()
{
    for(const auto& symbol : symbols)
    {
        const auto& symbolStr = symbol.first;
        size_t size = symbolStr.size();

        if(pos + size > sourceRef->getContent().size()) continue;

        if(std::string_view{sourceRef->getContent().data() + pos, size} != symbolStr) continue;
        tokens.push_back({.type = symbol.second, .source = sourceRef, .offset = pos, .size = size});

        pos += size;

        return true;
    }
    return false;
}

bool Lexer::isKeywords()
{
    for(const auto& keyword : keywords)
    {
        const auto& keywordStr = keyword.first;
        size_t size = keywordStr.size();

        if(pos + size > sourceRef->getContent().size()) continue;

        if(std::string_view{sourceRef->getContent().data() + pos, size} != keywordStr) continue;
        tokens.push_back({.type = keyword.second, .source = sourceRef, .offset = pos, .size = size});

        pos += size;

        return (!std::iswalnum(sourceRef->getContent()[pos]) && sourceRef->getContent()[pos] != '_') ||
               std::iswspace(sourceRef->getContent()[pos]);
    }
    return false;
}

bool Lexer::isInt()
{
    if(!std::iswdigit(sourceRef->getContent()[pos])) return false;

    size_t offset = pos;
    pos++;

    while(pos < sourceRef->getContent().size() && std::iswdigit(sourceRef->getContent()[pos])) pos++;

    size_t size = pos - offset;

    tokens.push_back({.type = TokenType::INTL, .source = sourceRef, .offset = offset, .size = size});

    return true;
}

bool Lexer::isStr()
{
    if(sourceRef->getContent()[pos] != '"') return false;

    size_t offset = pos;
    pos++;

    while(pos < sourceRef->getContent().size() && sourceRef->getContent()[pos] != '"' &&
          sourceRef->getContent()[pos] != '\n')
        pos++;

    pos++;
    size_t size = pos - offset;

    tokens.push_back({.type = TokenType::STRL, .source = sourceRef, .offset = offset, .size = size});

    return true;
}

bool Lexer::isIdent()
{
    if(!std::iswalpha(sourceRef->getContent()[pos]) && sourceRef->getContent()[pos] != '_') return false;

    size_t offset = pos;
    pos++;

    while(pos < sourceRef->getContent().size() &&
          (std::iswalnum(sourceRef->getContent()[pos]) || sourceRef->getContent()[pos] == '_'))
        pos++;

    size_t size = pos - offset;

    tokens.push_back({.type = TokenType::IDENT, .source = sourceRef, .offset = offset, .size = size});

    return true;
}

} // namespace Aerell