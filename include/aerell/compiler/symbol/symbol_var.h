#pragma once

#include "aerell/compiler/symbol/symbol.h"
#include "aerell/compiler/symbol/type.h"

namespace Aerell
{

class SymbolVar : public Symbol
{
  public:
    SymbolVar();
    ~SymbolVar();

    void setType(Type type);

  private:
    Type type;
};

} // namespace Aerell