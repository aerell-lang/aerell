#pragma once

#include <aerell/compiler/source/source.h>
#include <string_view>
#include <vector>

#include "aerell/compiler/token/token_type.h"

namespace Aerell
{

class Token
{
  public:
    TokenType type;
    Source* source = nullptr;
    size_t offset = 0;
    size_t size = 0;

    std::string_view getText() const;

  private:
};

void print(const std::vector<Token>& tokens);

} // namespace Aerell