#pragma once

#include <memory>
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
    bool jit(const char* filePath);
    bool compile(const char* filePath, std::vector<std::string>& outputs);

  private:
    SourceManager sourceManager;
    Source* mainSource = nullptr;

    Lexer lexer;

    SymbolTable symbolTable = {nullptr};
    Parser parser = {symbolTable};
    Semantic semantic{symbolTable};
    IR ir;

    std::vector<std::vector<Token>> genTokenss(Source* source);
    std::vector<std::unique_ptr<llvm::Module>> genIR(Source* source);
    std::vector<std::unique_ptr<llvm::Module>> genIR(const char* filePath);
};

} // namespace Aerell