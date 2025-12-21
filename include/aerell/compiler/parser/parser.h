#pragma once

#include <aerell/compiler/symbol/symbol_table.h>
#include <map>
#include <memory>
#include <vector>

#include "aerell/compiler/ast/ast.h"
#include "aerell/compiler/token/token.h"

namespace Aerell
{

class Parser
{
  public:
    Parser(SymbolTable& symbolTable);

    bool gen(const std::vector<Token>& tokens, std::vector<std::unique_ptr<AST>>& asts);

  private:
    enum class Rule
    {
        STMT,
        FUNC,
        FUNC_PARAM,
        DATA_TYPE,
        BLOCK,
        EXPR,
        FUNC_CALL,
        LITERAL
    };

    typedef std::map<Rule, std::vector<TokenType>> Rules;

    static Rules rules;
    SymbolTable* symbolTable = nullptr;
    const std::vector<Token>* tokensRef = nullptr;
    size_t pos = 0;
    bool hasError = false;

    void expectErrorMessage(const std::vector<TokenType>& types);
    void expectErrorMessage(Rule rule);
    bool expect(const std::vector<TokenType>& types);
    bool expect(TokenType type);
    bool expect(Rule rule);
    bool is(const std::vector<TokenType>& types);
    bool is(TokenType type);
    bool is(Rule rule);

    std::unique_ptr<AST> stmt();
    std::unique_ptr<AST> func();
    std::optional<FuncParam> funcParam();
    const Token* dataType();
    std::optional<std::vector<std::unique_ptr<AST>>> block();
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> funcCall();
    std::unique_ptr<AST> literal();
};

} // namespace Aerell