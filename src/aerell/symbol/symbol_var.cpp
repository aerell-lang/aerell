/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/symbol/symbol_type.h"
#include "aerell/symbol/symbol_var.h"

namespace aerell
{

SymbolVar::SymbolVar(SymbolTable* scope) : Symbol(scope, SymbolType::VAR) {}

SymbolVar::~SymbolVar() {}

void SymbolVar::setType(IRType type) { this->type = type; }

} // namespace aerell