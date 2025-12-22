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
    typedef std::vector<IR::Module> Modules;

    Tokens lexing(const char* filePath);
    bool parsing(const Tokens& cTokens, Asts& cAsts);
    bool analysis(const Asts& cAsts);
    bool generating(const Tokens& tokens, const Asts& cAsts, Modules& modules);

    bool jit(Modules& modules);
    bool compile(const Modules& modules, std::vector<std::string>& outputs);

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

void print(const Compiler::Modules& modules);

} // namespace Aerell