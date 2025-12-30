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
#include "aerell/compiler/ir/ir_type.h"

namespace aerell
{

class SymbolTable;

class SymbolFunc : public Symbol
{
  public:
    SymbolFunc(SymbolTable* scope, bool pub);
    ~SymbolFunc();

    const bool& getPub() const;
    void setVrdic(bool value);
    const bool& getVrdic() const;
    void setRet(IRType type);
    const std::optional<IRType>& getRet() const;
    void setParams(std::vector<IRType>&& types);
    const std::vector<IRType>& getParams() const;
    void setBlockScope(SymbolTable* blockScope);
    const SymbolTable* getBlockScope() const;

  private:
    bool pub;
    std::vector<IRType> params;
    bool vrdic = false;
    std::optional<IRType> ret = std::nullopt;
    SymbolTable* blockScope;
};

} // namespace aerell

#endif