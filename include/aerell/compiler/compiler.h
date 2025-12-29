/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_COMPILER_H
#define AERELL_COMPILER_COMPILER_H

#include <vector>

#include <aerell/compiler/source/source_manager.h>
#include "symbol/symbol_table.h"
#include <aerell/compiler/token/token.h>
#include <aerell/compiler/lexer/lexer.h>
#include <aerell/compiler/parser/parser.h>
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>

#include <llvm/IR/Module.h>

namespace Aerell
{

class Compiler
{
  public:
    const SymbolTable& getSymbolTable() const;
    bool lexing(const char* filePath, Token::Vecs& vecs);
    bool parsing(const Token::Vecs& vecs, AST::Groups& groups);
    bool analysis(const AST::Groups& groups);
    bool generating(const AST::Groups& groups, IR::Unit& unit);
    bool linking(IR::Unit& unit);
    bool jit(IR::Unit& unit);
    void optimize(IR::Unit& unit);
    bool compile(IR::Unit& unit, std::vector<std::string>& outputs);

  private:
    SourceManager sourceManager;
    Lexer lexer;
    SymbolTable symbolTable = {nullptr};
    Parser parser = {symbolTable};
    Semantic semantic{symbolTable};
    IR ir;

    bool lexing(Source* source, Token::Vecs& vecs);
    std::optional<std::string> compile(IR::Ptr& ptr);
    IR::Ptr linking(IR::Vec& vec);
    void optimize(IR::Ptr& ptr);
    void optimize(IR::Vec& vec);
    bool compile(IR::Vec& vec, std::vector<std::string>& outputs);
};

} // namespace Aerell

#endif