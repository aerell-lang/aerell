/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <aerell/compiler/symbol/symbol_var.h>
#include <aerell/compiler/symbol/symbol_func.h>
#include "aerell/compiler/symbol/symbol.h"

namespace Aerell
{

class SymbolTable
{
  public:
    SymbolTable(SymbolTable* parentScope);
    SymbolFunc* createFunc(bool pub, std::string_view ident);
    SymbolVar* createVar(std::string_view ident);
    SymbolTable* enterScope();
    SymbolTable* exitScope();
    SymbolFunc* findFunc(std::string_view ident);

  private:
    SymbolTable* parentScope;

    std::unordered_map<std::string_view, std::unique_ptr<Symbol>> symbols;
    std::vector<SymbolTable> scopes;
};

} // namespace Aerell