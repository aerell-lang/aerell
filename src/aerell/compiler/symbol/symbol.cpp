/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/symbol/symbol.h"

namespace Aerell
{

Symbol::Symbol(SymbolType type) : type(type) {}

Symbol::~Symbol() {}

} // namespace Aerell