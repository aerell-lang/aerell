#include <memory>
#include <sstream>
#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <aerell/compiler/ast/ast.h>
#include "aerell/compiler/parser/parser.h"

namespace Aerell
{

const std::vector<Token>* Parser::tokensRef = nullptr;
size_t Parser::pos = 0;
bool Parser::hasError = false;

bool Parser::gen(const std::vector<Token>& tokens, std::vector<std::unique_ptr<AST>>& asts)
{
    if(hasError) hasError = false;
    tokensRef = &tokens;

    while(pos < tokensRef->size() && (*tokensRef)[pos].type != TokenType::EOFF)
    {
        if(auto ast = expr()) asts.push_back(std::move(ast));
        else if(!is({TokenType::IDENT, TokenType::STRL}))
            pos++;
    }

    return !hasError;
}

void Parser::expectErrorMessage(const std::vector<TokenType>& types)
{
    std::ostringstream oss;

    oss << "Expected ";

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

bool Parser::expect(const std::vector<TokenType>& types)
{
    if(pos < tokensRef->size() &&
       std::any_of(types.begin(), types.end(), [](const auto& type) { return (*tokensRef)[pos].type != type; }))
    {
        expectErrorMessage(types);
        return false;
    }
    return true;
}

bool Parser::expect(TokenType type) { return expect(std::vector<TokenType>{type}); }

bool Parser::is(const std::vector<TokenType>& types)
{
    return pos < tokensRef->size() &&
           std::any_of(types.begin(), types.end(), [](const auto& type) { return (*tokensRef)[pos].type == type; });
}

bool Parser::is(TokenType type) { return is(std::vector<TokenType>{type}); }

std::unique_ptr<AST> Parser::expr()
{
    if(is(TokenType::IDENT)) return funcCall();
    if(is(TokenType::STRL)) return literal();
    expectErrorMessage({TokenType::IDENT, TokenType::STRL});
    return nullptr;
}

std::unique_ptr<AST> Parser::funcCall()
{
    // IDENT
    if(!expect(TokenType::IDENT)) return nullptr;
    auto name = &(*tokensRef)[pos];
    pos++;

    // LPAREN
    if(!expect(TokenType::LPAREN)) return nullptr;
    pos++;

    // args
    std::vector<std::unique_ptr<AST>> args;
    if(is({TokenType::IDENT, TokenType::STRL}))
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
    funcCall->name = name;
    funcCall->args = std::move(args);

    return funcCall;
}

std::unique_ptr<AST> Parser::literal()
{
    // STRL
    if(!expect(TokenType::STRL)) return nullptr;
    auto value = &(*tokensRef)[pos];
    pos++;

    // Gen AST
    auto literal = std::make_unique<Literal>();
    literal->value = value;

    return literal;
}

} // namespace Aerell