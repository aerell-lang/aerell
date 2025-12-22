#include <aerell/compiler/symbol/symbol_func.h>
#include <memory>
#include <sstream>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <aerell/compiler/ast/ast.h>
#include "aerell/compiler/parser/parser.h"

namespace Aerell
{

Parser::Rules Parser::rules{
    {Rule::STMT, {TokenType::F, TokenType::PF, TokenType::IDENT, TokenType::INTL, TokenType::STRL}},
    {Rule::FUNC, {TokenType::F, TokenType::PF}},
    {Rule::FUNC_PARAM, {TokenType::IDENT}},
    {Rule::DATA_TYPE, {TokenType::I32, TokenType::STR}},
    {Rule::BLOCK, {TokenType::LBRACE}},
    {Rule::EXPR, {TokenType::IDENT, TokenType::INTL, TokenType::STRL}},
    {Rule::FUNC_CALL, {TokenType::IDENT}},
    {Rule::LITERAL, {TokenType::INTL, TokenType::STRL}},
};

Parser::Parser(SymbolTable& symbolTable) : symbolTable(&symbolTable) {}

bool Parser::parsing(const Lexer::Tokens& tokens, AST::Asts& asts)
{
    if(this->hasError) this->hasError = false;
    this->tokensRef = &tokens;
    this->pos = 0;

    while(this->pos < this->tokensRef->size() && (*this->tokensRef)[this->pos].type != TokenType::EOFF)
    {
        if(is(Rule::STMT))
        {
            if(auto ast = stmt())
            {
                asts.push_back(std::move(ast));
                continue;
            }
        }
        expectErrorMessage(Rule::STMT);
        this->pos++;
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
        oss << to_string(type);
    }

    oss << " but instead " << to_string((*tokensRef)[pos].type);

    (*tokensRef)[pos].source->printErrorMessage((*tokensRef)[pos].offset, (*tokensRef)[pos].size, oss.str().c_str());

    if(!hasError) hasError = true;
}

void Parser::expectErrorMessage(Rule rule) { return expectErrorMessage(rules[rule]); }

bool Parser::expect(const std::vector<TokenType>& types)
{
    if(pos < tokensRef->size() &&
       std::all_of(types.begin(), types.end(), [&](const auto& type) { return (*tokensRef)[pos].type != type; }))
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
           std::any_of(types.begin(), types.end(), [&](const auto& type) { return (*tokensRef)[pos].type == type; });
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
    // P / PF
    if(!expect(Rule::FUNC)) return nullptr;
    bool pub = is(TokenType::PF);
    pos++;

    // IDENT
    if(!expect(TokenType::IDENT)) return nullptr;
    auto ident = &(*tokensRef)[pos];
    pos++;

    auto symbolFunc = this->symbolTable->findFunc(ident->getText());
    if(symbolFunc != nullptr)
    {
        ident->source->printErrorMessage(ident->offset, ident->size, "[P] Duplicate function");
        return nullptr;
    };
    symbolFunc = this->symbolTable->createFunc(pub, ident->getText());

    // LPAREN
    if(!expect(TokenType::LPAREN)) return nullptr;
    pos++;

    auto* funcSymbolTable = this->symbolTable->enterScope();
    this->symbolTable = funcSymbolTable;

    // Params
    std::vector<FuncParam> params;
    std::vector<Type> types;
    if(is(Rule::FUNC_PARAM))
        if(auto param = funcParam())
        {
            if(param.value().type->type == TokenType::I32) types.push_back(Type::I32);
            else if(param.value().type->type == TokenType::STR)
                types.push_back(Type::STR);

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
                    if(param.value().type->type == TokenType::I32) types.push_back(Type::I32);
                    else if(param.value().type->type == TokenType::STR)
                        types.push_back(Type::STR);

                    params.push_back(std::move(param.value()));
                }
            }
        }

    symbolFunc->setParams(std::move(types));

    this->symbolTable = this->symbolTable->exitScope();

    // RPAREN
    if(!expect(TokenType::RPAREN)) return nullptr;
    pos++;

    // Ret
    const Token* ret = nullptr;
    if(is(Rule::DATA_TYPE)) ret = dataType();
    if(ret == nullptr) symbolFunc->setRet(Type::VOID);
    else if(ret->type == TokenType::I32)
        symbolFunc->setRet(Type::I32);
    else if(ret->type == TokenType::STR)
        symbolFunc->setRet(Type::STR);

    // Block
    std::optional<std::vector<std::unique_ptr<AST>>> stmts = std::nullopt;
    if(is(Rule::BLOCK))
    {
        this->symbolTable = funcSymbolTable;
        stmts = block();
        this->symbolTable = this->symbolTable->exitScope();
        symbolFunc->setScope(funcSymbolTable);
    }

    // Gen AST
    auto func = std::make_unique<Func>();
    func->ident = ident;
    func->params = std::move(params);
    func->ret = ret;
    func->stmts = std::move(stmts);
    func->symbol = symbolFunc;

    return func;
}

std::optional<FuncParam> Parser::funcParam()
{
    // IDENT
    if(!expect(Rule::FUNC_PARAM)) return std::nullopt;
    auto ident = &(*tokensRef)[pos];
    pos++;

    auto symbolVar = this->symbolTable->createVar(ident->getText());

    // DATA_TYPE
    if(!is(Rule::DATA_TYPE)) return std::nullopt;
    const Token* type{dataType()};
    if(type != nullptr)
    {
        if(type->type == TokenType::I32) symbolVar->setType(Type::I32);
        else if(type->type == TokenType::STR)
            symbolVar->setType(Type::STR);
    }

    // Gen AST
    FuncParam param;
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
    auto indent = &(*tokensRef)[pos];
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
    auto funcCall = std::make_unique<FuncCall>();
    funcCall->ident = indent;
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
    auto literal = std::make_unique<Literal>();
    literal->value = value;

    return literal;
}

} // namespace Aerell