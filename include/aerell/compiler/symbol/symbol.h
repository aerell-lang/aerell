#pragma once

#include "symbol_type.h"

namespace Aerell
{

class Symbol
{
  public:
    Symbol(SymbolType type);
    virtual ~Symbol();

  private:
    SymbolType type;
};

} // namespace Aerell