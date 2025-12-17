#pragma once

#include <memory>
#include <vector>

#include <aerell/compiler/ast/ast.h>

namespace Aerell
{

class Semantic
{
  public:
    static bool analysis(std::vector<std::unique_ptr<AST>>& asts);

  private:
    static bool hasError;

    static TokenType expr(std::unique_ptr<AST>& ast);
    static TokenType funcCall(FuncCall& ctx);
    static TokenType literal(Literal& ctx);
};

} // namespace Aerell