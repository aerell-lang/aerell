#pragma once

#include <aerell/compiler/symbol/symbol_table.h>
#include <memory>

#include <aerell/compiler/ast/ast.h>

namespace Aerell
{

class Semantic
{
  public:
    Semantic(SymbolTable& symbolTable);
    bool analysis(const AST::Asts& asts);

  private:
    bool hasError = false;
    SymbolTable* symbolTable;

    Type expr(const std::unique_ptr<AST>& ast);

    void func(Func& ctx);
    Type funcCall(FuncCall& ctx);
    Type literal(Literal& ctx);
};

} // namespace Aerell