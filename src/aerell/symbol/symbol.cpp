/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/symbol/symbol.h"

namespace aerell
{

Symbol::Symbol(SymbolTable* scope, SymbolType symbolType) : scope(scope), symbolType(symbolType) {}

Symbol::~Symbol() {}

const SymbolType& Symbol::getSymbolType() const { return this->symbolType; }

const SymbolTable* Symbol::getScope() const { return this->scope; }

} // namespace aerell