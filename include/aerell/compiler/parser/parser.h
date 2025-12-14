#pragma once

#include <memory>
#include <vector>

#include "aerell/compiler/ast/ast.h"
#include "aerell/compiler/token/token.h"

namespace aerell
{

class Parser
{
  public:
    static std::vector<std::unique_ptr<AST>> gen(const std::vector<Token>& input);

  private:
    static const std::vector<Token>* tokens;
    static size_t pos;

    static std::unique_ptr<AST> expr();
    static std::unique_ptr<AST> funcCall();
    static std::unique_ptr<AST> literal();
};

} // namespace aerell