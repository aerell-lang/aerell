#pragma once

#include <utility>
#include <vector>

#include "aerell/compiler/token/token.h"

namespace Aerell
{

class Lexer
{
  public:
    static std::vector<Token> gen(const std::string& input);

  private:
    static const std::pair<std::string, TokenType> symbols[];

    static std::string content;
    static size_t pos;

    static std::vector<Token> tokens;

    static bool isWS();
    static bool isComment();
    static bool isSymbl();
    static bool isStr();
    static bool isIdent();
};

} // namespace Aerell