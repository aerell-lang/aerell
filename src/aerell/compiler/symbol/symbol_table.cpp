#include "aerell/compiler/symbol/symbol_table.h"
#include <memory>

namespace Aerell
{

SymbolTable::SymbolTable(SymbolTable* parentScope) : parentScope(parentScope) {}

SymbolFunc* SymbolTable::createFunc(bool pub, std::string_view ident)
{
    auto symbol = std::make_unique<SymbolFunc>(pub);
    auto* symbolRaw = symbol.get();
    this->symbols[ident] = std::move(symbol);
    return symbolRaw;
}

SymbolVar* SymbolTable::createVar(std::string_view ident)
{
    auto symbol = std::make_unique<SymbolVar>();
    auto* symbolRaw = symbol.get();
    this->symbols[ident] = std::move(symbol);
    return symbolRaw;
}

SymbolTable* SymbolTable::enterScope() { return &this->scopes.emplace_back(this); }

SymbolTable* SymbolTable::exitScope() { return this->parentScope; }

SymbolFunc* SymbolTable::findFunc(std::string_view ident)
{
    if(this->symbols.contains(ident))
        if(auto* symbol = dynamic_cast<SymbolFunc*>(this->symbols[ident].get())) return symbol;

    if(this->parentScope == nullptr) return nullptr;

    return this->parentScope->findFunc(ident);
}

} // namespace Aerell