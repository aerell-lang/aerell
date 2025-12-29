/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_SYMBOL_SYMBOL_TABLE_H
#define AERELL_COMPILER_SYMBOL_SYMBOL_TABLE_H

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "aerell/compiler/symbol/symbol_var.h"
#include "aerell/compiler/symbol/symbol_func.h"
#include "aerell/compiler/symbol/symbol.h"

namespace aerell
{

class SymbolTable
{
  public:
    typedef std::unordered_map<std::string_view, std::unique_ptr<Symbol>> Symbols;
    typedef std::vector<SymbolTable> Scopes;

    SymbolTable(SymbolTable* parentScope);

    const Symbols& getSymbols() const;
    const std::vector<SymbolTable>& getScopes() const;
    const SymbolTable* getParentScope() const;

    SymbolFunc* createFunc(bool pub, std::string_view ident);
    SymbolVar* createVar(std::string_view ident);
    SymbolTable* enterScope();
    SymbolTable* exitScope();
    SymbolFunc* findFunc(std::string_view ident, bool recursive) const;

  private:
    SymbolTable* parentScope;

    Symbols symbols;
    Scopes scopes;
};

} // namespace aerell

#endif