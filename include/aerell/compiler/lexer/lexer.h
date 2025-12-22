#pragma once

#include <utility>
#include <vector>

#include "aerell/compiler/token/token.h"
#include <aerell/compiler/source/source.h>

namespace Aerell
{

class Lexer
{
  public:
    typedef std::vector<Token> Tokens;

    Tokens lexing(Source* source);

  private:
    static const std::pair<std::string, TokenType> symbols[];
    static const std::pair<std::string, TokenType> keywords[];

    Source* sourceRef = nullptr;
    size_t pos = 0;
    Tokens tokens;

    bool isWS();
    bool isComment();
    bool isSymbols();
    bool isKeywords();
    bool isInt();
    bool isStr();
    bool isIdent();
};

} // namespace Aerell