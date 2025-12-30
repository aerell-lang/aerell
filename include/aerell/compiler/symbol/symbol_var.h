/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SYMBOL_SYMBOL_VAR_H
#define AERELL_COMPILER_SYMBOL_SYMBOL_VAR_H

#include "aerell/compiler/symbol/symbol.h"
#include <aerell/compiler/ir/ir_type.h>

namespace aerell
{

class SymbolVar : public Symbol
{
  public:
    SymbolVar(SymbolTable* scope);
    ~SymbolVar();

    void setType(IRType type);

  private:
    IRType type;
};

} // namespace aerell

#endif