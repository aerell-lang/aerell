#pragma once

#include <string>
#include <vector>

#include "aerell/compiler/token/token_type.h"

namespace Aerell
{

struct Token
{
    TokenType type;
    std::string content;
};

void print(const Token& token);

void print(const std::vector<Token>& tokens);

} // namespace Aerell