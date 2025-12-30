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

namespace aerell
{

class Compiler
{
  public:
    const SymbolTable& getSymbolTable() const;
    bool lexing(const char* filePath, Token::Vecs& vecs);
    bool parsing(const Token::Vecs& vecs, AST::Groups& groups);
    bool analysis(const AST::Groups& groups);
    bool generating(const AST::Groups& groups, IRMod::Vec& vec);
    bool generating(const IRMod::Vec& vec, IRllvm::Unit& unit);
    bool linking(IRllvm::Unit& unit);
    bool jit(IRllvm::Unit& unit);
    void optimize(IRllvm::Unit& unit);
    bool compile(IRllvm::Unit& unit, std::vector<std::string>& outputs);

  private:
    SourceManager sourceManager;
    Lexer lexer;
    SymbolTable symbolTable = {nullptr};
    Parser parser = {symbolTable};
    Semantic semantic{symbolTable};
    IR ir;

    bool lexing(Source* source, Token::Vecs& vecs);
    std::optional<std::string> compile(IRllvm::Ptr& ptr);
    IRllvm::Ptr linking(IRllvm::Vec& vec);
    void optimize(IRllvm::Ptr& ptr);
    void optimize(IRllvm::Vec& vec);
    bool compile(IRllvm::Vec& vec, std::vector<std::string>& outputs);
};

} // namespace aerell

#endif