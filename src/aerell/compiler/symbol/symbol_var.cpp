/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/symbol/symbol_var.h"
#include <aerell/compiler/symbol/symbol_type.h>

namespace Aerell
{

SymbolVar::SymbolVar() : Symbol(SymbolType::VAR) {}

SymbolVar::~SymbolVar() {}

void SymbolVar::setType(Type type) { this->type = type; }

} // namespace Aerell