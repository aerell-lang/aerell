/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SYMBOL_SYMBOL_FUNC_H
#define AERELL_COMPILER_SYMBOL_SYMBOL_FUNC_H

#include <optional>
#include <vector>

#include "aerell/compiler/symbol/symbol.h"
#include "aerell/compiler/symbol/data_type.h"

namespace Aerell
{

class SymbolTable;

class SymbolFunc : public Symbol
{
  public:
    SymbolFunc(SymbolTable* scope, bool pub);
    ~SymbolFunc();

    bool getPub() const;
    void setVrdic(bool value);
    bool getVrdic() const;
    void setRet(DataType dataType);
    const std::optional<DataType>& getRet() const;
    void setParams(std::vector<DataType>&& dataTypes);
    const std::vector<DataType>& getParams() const;
    void setBlockScope(SymbolTable* blockScope);
    const SymbolTable* getBlockScope() const;

  private:
    bool pub;
    std::vector<DataType> params;
    bool vrdic = false;
    std::optional<DataType> ret;
    SymbolTable* blockScope;
};

} // namespace Aerell

#endif