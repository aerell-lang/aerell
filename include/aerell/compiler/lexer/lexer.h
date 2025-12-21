#pragma once

#include <aerell/compiler/source/source.h>
#include <utility>
#include <vector>

#include "aerell/compiler/token/token.h"

namespace Aerell
{

class Lexer
{
  public:
    std::vector<Token> gen(Source* source);

  private:
    static const std::pair<std::string, TokenType> symbols[];
    static const std::pair<std::string, TokenType> keywords[];

    Source* sourceRef = nullptr;
    size_t pos = 0;
    std::vector<Token> tokens;

    bool isWS();
    bool isComment();
    bool isSymbols();
    bool isKeywords();
    bool isInt();
    bool isStr();
    bool isIdent();
};

} // namespace Aerell