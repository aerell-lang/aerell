/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

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
    typedef std::vector<Lexer::Tokens> Tokens;
    typedef std::vector<AST::Asts> Asts;

    Tokens lexing(const char* filePath);
    bool parsing(const Tokens& cTokens, Asts& cAsts);
    bool analysis(const Asts& cAsts);
    bool generating(const Tokens& tokens, const Asts& cAsts, IR::Modules& modules);
    bool jit(IR::Modules& modules);
    IR::Module linking(IR::Modules& modules);
    bool compile(IR::Modules& modules, std::vector<std::string>& outputs);
    std::optional<std::string> compile(IR::Modules& modules);

  private:
    SourceManager sourceManager;
    Lexer lexer;
    SymbolTable symbolTable = {nullptr};
    Parser parser = {symbolTable};
    Semantic semantic{symbolTable};
    IR ir;

    Tokens lexing(Source* source);
};

void print(const Compiler::Tokens& cTokens);

void print(const Compiler::Asts& cAsts);

void print(const IR::Modules& modules);

} // namespace Aerell