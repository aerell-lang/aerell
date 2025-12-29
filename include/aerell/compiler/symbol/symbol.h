/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SYMBOL_SYMBOL_H
#define AERELL_COMPILER_SYMBOL_SYMBOL_H

#include "symbol_type.h"

namespace aerell
{

class SymbolTable;
class Symbol
{
  public:
    Symbol(SymbolTable* scope, SymbolType symbolType);
    virtual ~Symbol();

    const SymbolType& getSymbolType() const;
    const SymbolTable* getScope() const;

  private:
    SymbolTable* scope;
    SymbolType symbolType;
};

} // namespace aerell

#endif