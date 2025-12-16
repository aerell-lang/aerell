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
    static std::vector<Token> gen(Source* source);

  private:
    static const std::pair<std::string, TokenType> symbols[];

    static Source* sourceRef;
    static size_t pos;
    static std::vector<Token> tokens;

    static bool isWS();
    static bool isComment();
    static bool isSymbl();
    static bool isStr();
    static bool isIdent();
};

} // namespace Aerell