#pragma once

#include <memory>
#include <vector>

#include "aerell/compiler/ast/ast.h"
#include "aerell/compiler/token/token.h"

namespace Aerell
{

class Parser
{
  public:
    static bool gen(const std::vector<Token>& tokens, std::vector<std::unique_ptr<AST>>& asts);

  private:
    static const std::vector<Token>* tokensRef;
    static size_t pos;
    static bool hasError;

    static void expectErrorMessage(const std::vector<TokenType>& types);
    static bool expect(const std::vector<TokenType>& types);
    static bool expect(TokenType type);
    static bool is(const std::vector<TokenType>& types);
    static bool is(TokenType type);

    static std::unique_ptr<AST> expr();
    static std::unique_ptr<AST> funcCall();
    static std::unique_ptr<AST> literal();
};

} // namespace Aerell