/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <memory>

#include "aerell/symbol/symbol_table.h"

namespace aerell
{

const SymbolTable::Symbols& SymbolTable::getSymbols() const { return this->symbols; }

const SymbolTable::Scopes& SymbolTable::getScopes() const { return this->scopes; }

const SymbolTable* SymbolTable::getParentScope() const { return this->parentScope; }

SymbolTable::SymbolTable(SymbolTable* parentScope) : parentScope(parentScope) {}

SymbolFunc* SymbolTable::createFunc(bool pub, std::string_view ident)
{
    auto symbol = std::make_unique<SymbolFunc>(this, pub);
    auto* symbolRaw = symbol.get();
    this->symbols[ident] = std::move(symbol);
    return symbolRaw;
}

SymbolVar* SymbolTable::createVar(std::string_view ident)
{
    auto symbol = std::make_unique<SymbolVar>(this);
    auto* symbolRaw = symbol.get();
    this->symbols[ident] = std::move(symbol);
    return symbolRaw;
}

SymbolTable* SymbolTable::enterScope() { return this->scopes.emplace_back(std::make_unique<SymbolTable>(this)).get(); }

SymbolTable* SymbolTable::exitScope() { return this->parentScope; }

SymbolFunc* SymbolTable::findFunc(std::string_view ident, bool recursive) const
{
    const auto& symbol = this->symbols.find(ident);
    if(symbol != this->symbols.end())
        if(auto* symbolFunc = dynamic_cast<SymbolFunc*>(symbol->second.get())) return symbolFunc;

    if(!recursive || this->parentScope == nullptr) return nullptr;

    return this->parentScope->findFunc(ident, recursive);
}

} // namespace aerell