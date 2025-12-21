#pragma once

#include "aerell/compiler/symbol/symbol.h"
#include "type.h"
#include <vector>

namespace Aerell
{

class SymbolTable;

class SymbolFunc : public Symbol
{
  public:
    SymbolFunc(bool pub);
    ~SymbolFunc();

    bool getPub() const;
    void setVrdic(bool value);
    bool getVrdic() const;
    void setRet(Type type);
    Type getRet() const;
    void setParams(std::vector<Type>&& types);
    const std::vector<Type>& getParams() const;
    void setScope(SymbolTable* scope);

  private:
    bool pub;
    std::vector<Type> params;
    bool vrdic = false;
    Type ret;
    SymbolTable* scope;
};

} // namespace Aerell