/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_PARSER_PARSER_H
#define AERELL_COMPILER_PARSER_PARSER_H

#include <map>
#include <memory>
#include <vector>

#include <aerell/compiler/symbol/symbol_table.h>
#include <aerell/compiler/lexer/lexer.h>
#include "aerell/compiler/ast/ast.h"
#include "aerell/compiler/ast/ast_func_param.h"
#include "aerell/compiler/token/token.h"

namespace aerell
{

class Parser
{
  public:
    Parser(SymbolTable& symbolTable);

    bool parsing(const Token::Vec& vec, AST::ChildrenWithSource& childrenWithSource);

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
    std::optional<ASTFuncParam> funcParam();
    const Token* dataType();
    std::optional<std::vector<std::unique_ptr<AST>>> block();
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> funcCall();
    std::unique_ptr<AST> literal();
};

} // namespace aerell

#endif