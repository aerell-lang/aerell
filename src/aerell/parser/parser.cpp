/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include "aerell/parser/parser.h"
#include "aerell/symbol/symbol_func.h"
#include "aerell/ast/ast_func.h"
#include "aerell/ast/ast_func_call.h"
#include "aerell/ast/ast_literal.h"

namespace aerell
{

static std::map<Parser::Rule, std::vector<TokenType>> rules{
    {Parser::Rule::STMT, {TokenType::IDENT, TokenType::INTL, TokenType::FLTL, TokenType::CHRL, TokenType::STRL}},
    {Parser::Rule::FUNC, {TokenType::IDENT}},
    {Parser::Rule::FUNC_PARAM, {TokenType::IDENT}},
    {Parser::Rule::DATA_TYPE, {TokenType::I32, TokenType::F32, TokenType::CHR, TokenType::STR}},
    {Parser::Rule::BLOCK, {TokenType::LBRACE}},
    {Parser::Rule::EXPR, {TokenType::IDENT, TokenType::INTL, TokenType::FLTL, TokenType::CHRL, TokenType::STRL}},
    {Parser::Rule::FUNC_CALL, {TokenType::IDENT}},
    {Parser::Rule::LITERAL, {TokenType::INTL, TokenType::FLTL, TokenType::CHRL, TokenType::STRL}},
};

Parser::Parser(SymbolTable& symbolTable) : symbolTable(&symbolTable) {}

bool Parser::parsing(const Lexer::Result& lResult, Result& pResult)
{
    if(this->hasError) this->hasError = false;
    this->tokensRef = &lResult.tokens;
    this->pos = 0;

    pResult.source = lResult.source;
    while(this->pos < this->tokensRef->size() && (*this->tokensRef)[this->pos].type() != TokenType::EOFF)
    {
        if(is(Rule::STMT))
        {
            if(auto ast = stmt())
            {
                pResult.asts.emplace_back(std::move(ast));
                continue;
            }
        }
        expectErrorMessage(Rule::STMT);
        this->pos++;
    }

    return !this->hasError;
}

bool Parser::parsing(const Lexer::Results& lResults, Results& pResults)
{
    for(const auto& lResult : lResults)
    {
        Result result;
        if(!this->parsing(lResult, result)) continue;
        pResults.emplace_back(std::move(result));
    }

    return !this->hasError;
}

void Parser::expectErrorMessage(const std::vector<TokenType>& types)
{
    std::ostringstream oss;

    oss << "[P] Expected ";

    bool init = true;
    for(const auto& type : types)
    {
        if(init) init = false;
        else
            oss << ", ";
        oss << toString(type);
    }

    oss << " but instead " << toString((*tokensRef)[pos].type());

    (*tokensRef)[pos].print(oss.str().c_str());

    if(!hasError) hasError = true;
}

void Parser::expectErrorMessage(Rule rule) { return expectErrorMessage(rules[rule]); }

bool Parser::expect(const std::vector<TokenType>& types)
{
    if(pos < tokensRef->size() &&
       std::all_of(types.begin(), types.end(), [&](const auto& type) { return (*tokensRef)[pos].type() != type; }))
    {
        expectErrorMessage(types);
        return false;
    }
    return true;
}

bool Parser::expect(TokenType type) { return expect(std::vector<TokenType>{type}); }

bool Parser::expect(Rule rule) { return expect(rules[rule]); }

bool Parser::is(const std::vector<TokenType>& types)
{
    return pos < tokensRef->size() &&
           std::any_of(types.begin(), types.end(), [&](const auto& type) { return (*tokensRef)[pos].type() == type; });
}

bool Parser::is(TokenType type) { return is(std::vector<TokenType>{type}); }

bool Parser::is(Rule rule) { return is(rules[rule]); }

std::unique_ptr<AST> Parser::stmt()
{
    if(is(Rule::FUNC)) return func();
    if(is(Rule::EXPR)) return expr();
    expectErrorMessage(Rule::STMT);
    return nullptr;
}

std::unique_ptr<AST> Parser::func()
{
    // IDENT
    if(!expect(TokenType::IDENT)) return nullptr;
    auto ident = &(*tokensRef)[pos];
    bool pub = true;
    pos++;

    auto symbolFunc = this->symbolTable->findFunc(ident->lexeme(), false);
    if(symbolFunc != nullptr)
    {
        ident->print("[P] Duplicate function");
        return nullptr;
    };
    symbolFunc = this->symbolTable->createFunc(pub, ident->lexeme());

    // LPAREN
    if(!expect(TokenType::LPAREN)) return nullptr;
    pos++;

    auto* funcSymbolTable = this->symbolTable->enterScope();
    this->symbolTable = funcSymbolTable;

    // Params
    std::vector<ASTFuncParam> params;
    std::vector<IRType> dataTypes;
    if(is(Rule::FUNC_PARAM))
        if(auto param = funcParam())
        {
            if(param.value().type->type() == TokenType::I32) dataTypes.push_back(IRType::I32);
            else if(param.value().type->type() == TokenType::F32)
                dataTypes.push_back(IRType::F32);
            else if(param.value().type->type() == TokenType::CHR)
                dataTypes.push_back(IRType::CHR);
            else if(param.value().type->type() == TokenType::STR)
                dataTypes.push_back(IRType::STR);

            params.push_back(std::move(param.value()));
            while(is(TokenType::COMMA))
            {
                pos++;
                // Variadic
                if(is(TokenType::VRDIC))
                {
                    pos++;
                    symbolFunc->setVrdic(true);
                    break;
                }
                else if(auto param = funcParam())
                {
                    if(param.value().type->type() == TokenType::I32) dataTypes.push_back(IRType::I32);
                    else if(param.value().type->type() == TokenType::F32)
                        dataTypes.push_back(IRType::F32);
                    else if(param.value().type->type() == TokenType::CHR)
                        dataTypes.push_back(IRType::CHR);
                    else if(param.value().type->type() == TokenType::STR)
                        dataTypes.push_back(IRType::STR);

                    params.push_back(std::move(param.value()));
                }
            }
        }

    symbolFunc->setParams(std::move(dataTypes));

    this->symbolTable = this->symbolTable->exitScope();

    // RPAREN
    if(!expect(TokenType::RPAREN)) return nullptr;
    pos++;

    // Ret
    const Token* ret = nullptr;
    if(is(Rule::DATA_TYPE)) ret = dataType();
    if(ret != nullptr)
    {
        if(ret->type() == TokenType::I32) symbolFunc->setRet(IRType::I32);
        if(ret->type() == TokenType::F32) symbolFunc->setRet(IRType::F32);
        if(ret->type() == TokenType::CHR) symbolFunc->setRet(IRType::CHR);
        if(ret->type() == TokenType::STR) symbolFunc->setRet(IRType::STR);
    }

    // Block
    std::optional<std::vector<std::unique_ptr<AST>>> stmts = std::nullopt;
    if(is(Rule::BLOCK))
    {
        this->symbolTable = funcSymbolTable;
        stmts = block();
        this->symbolTable = this->symbolTable->exitScope();
        symbolFunc->setBlockScope(funcSymbolTable);
    }

    // Gen AST
    auto func = std::make_unique<ASTFunc>();
    func->ident = ident;
    func->params = std::move(params);
    func->ret = ret;
    func->stmts = std::move(stmts);
    func->symbol = symbolFunc;

    return func;
}

std::optional<ASTFuncParam> Parser::funcParam()
{
    // IDENT
    if(!expect(Rule::FUNC_PARAM)) return std::nullopt;
    auto ident = &(*tokensRef)[pos];
    pos++;

    auto symbolVar = this->symbolTable->createVar(ident->lexeme());

    // DATA_TYPE
    if(!is(Rule::DATA_TYPE)) return std::nullopt;
    const Token* type{dataType()};
    if(type != nullptr)
    {
        if(type->type() == TokenType::I32) symbolVar->setType(IRType::I32);
        else if(type->type() == TokenType::F32)
            symbolVar->setType(IRType::F32);
        else if(type->type() == TokenType::CHR)
            symbolVar->setType(IRType::CHR);
        else if(type->type() == TokenType::STR)
            symbolVar->setType(IRType::STR);
        else
            return std::nullopt;
    }

    // Gen AST
    ASTFuncParam param;
    param.ident = ident;
    param.type = type;

    return param;
}

const Token* Parser::dataType()
{
    // DATA_TYPE
    if(!expect(Rule::DATA_TYPE)) return nullptr;
    auto type = &(*tokensRef)[pos];
    pos++;

    return type;
}

std::optional<std::vector<std::unique_ptr<AST>>> Parser::block()
{
    // LBRACE
    if(!expect(Rule::BLOCK)) return std::nullopt;
    pos++;

    std::vector<std::unique_ptr<AST>> block;
    while(is(Rule::STMT))
        if(auto ast = stmt()) block.push_back(std::move(ast));

    // RBRACE
    if(!expect(TokenType::RBRACE)) return std::nullopt;
    pos++;

    return block;
}

std::unique_ptr<AST> Parser::expr()
{
    if(is(Rule::FUNC_CALL)) return funcCall();
    if(is(Rule::LITERAL)) return literal();
    expectErrorMessage(Rule::EXPR);
    return nullptr;
}

std::unique_ptr<AST> Parser::funcCall()
{
    // IDENT
    if(!expect(Rule::FUNC_CALL)) return nullptr;
    auto ident = &(*tokensRef)[pos];
    pos++;

    // LPAREN
    if(!expect(TokenType::LPAREN)) return nullptr;
    pos++;

    // args
    std::vector<std::unique_ptr<AST>> args;
    if(is(Rule::EXPR))
        if(auto arg = expr())
        {
            args.push_back(std::move(arg));
            while(is(TokenType::COMMA))
            {
                pos++;
                if(auto arg = expr()) args.push_back(std::move(arg));
            }
        }

    // RPAREN
    if(!expect(TokenType::RPAREN)) return nullptr;
    pos++;

    // Gen AST
    auto funcCall = std::make_unique<ASTFuncCall>();
    funcCall->ident = ident;
    funcCall->args = std::move(args);

    return funcCall;
}

std::unique_ptr<AST> Parser::literal()
{
    // STRL
    if(!expect(Rule::LITERAL)) return nullptr;
    auto value = &(*tokensRef)[pos];
    pos++;

    // Gen AST
    auto literal = std::make_unique<ASTLiteral>();
    literal->value = value;

    return literal;
}

} // namespace aerell