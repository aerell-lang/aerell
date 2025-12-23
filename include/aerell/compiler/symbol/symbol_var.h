/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

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