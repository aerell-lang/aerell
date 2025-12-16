#include <memory>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <aerell/compiler/ast/ast.h>
#include "aerell/compiler/parser/parser.h"

namespace Aerell
{

const std::vector<Token>* Parser::tokens = nullptr;
size_t Parser::pos = 0;

std::vector<std::unique_ptr<AST>> Parser::gen(const std::vector<Token>& input)
{
    tokens = &input;

    std::vector<std::unique_ptr<AST>> asts;

    while(pos < tokens->size())
        if(auto ast = expr()) asts.push_back(std::move(ast));

    return asts;
}

std::unique_ptr<AST> Parser::expr()
{
    if(auto ast = funcCall()) return ast;
    if(auto ast = literal()) return ast;
    return nullptr;
}

std::unique_ptr<AST> Parser::funcCall()
{
    // IDENT
    if(pos < tokens->size() && (*tokens)[pos].type != TokenType::IDENT) return nullptr;
    std::string name = (*tokens)[pos].content;
    pos++;

    // LPAREN
    if(pos < tokens->size() && (*tokens)[pos].type != TokenType::LPAREN) return nullptr;
    pos++;

    // args
    std::vector<std::unique_ptr<AST>> args;
    if(auto arg = expr())
    {
        args.push_back(std::move(arg));
        while(pos < tokens->size() && (*tokens)[pos].type == TokenType::COMMA)
        {
            pos++;
            if(auto arg = expr()) args.push_back(std::move(arg));
        }
    }

    // RPAREN
    if(pos < tokens->size() && (*tokens)[pos].type != TokenType::RPAREN) return nullptr;
    pos++;

    // Gen AST
    auto funcCall = std::make_unique<FuncCall>();
    funcCall->name = name;
    funcCall->args = std::move(args);

    return funcCall;
}

std::unique_ptr<AST> Parser::literal()
{
    // STRL
    if(pos < tokens->size() && (*tokens)[pos].type != TokenType::STRL) return nullptr;
    std::string value = (*tokens)[pos].content;
    pos++;

    // Gen AST
    auto literal = std::make_unique<Literal>();
    literal->value = value;

    return literal;
}

} // namespace Aerell