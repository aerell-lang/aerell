/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/symbol/symbol_print.h"
#include "aerell/compiler/symbol/symbol_type.h"

namespace aerell
{

void print(const SymbolTable& symbolTable)
{
    llvm::outs() << "\nSymbolTable: " << &symbolTable << '\n';
    llvm::outs() << "ParentScope: " << symbolTable.getParentScope() << '\n';
    const auto& symbols = symbolTable.getSymbols();
    if(!symbols.empty())
    {
        llvm::outs() << "name, type\n";
        for(const auto& symbol : symbols)
            llvm::outs() << symbol.first << ", " << to_string(symbol.second->getSymbolType()) << '\n';
    }
    for(const auto& scope : symbolTable.getScopes()) print(*scope);
}

} // namespace aerell