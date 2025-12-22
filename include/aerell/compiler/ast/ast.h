#pragma once

#include <memory>
#include <vector>

#include <aerell/compiler/symbol/symbol_func.h>
#include <aerell/compiler/symbol/symbol_var.h>
#include <aerell/compiler/token/token.h>

namespace Aerell
{

class AST
{
  public:
    typedef std::unique_ptr<AST> Ast;
    typedef std::vector<Ast> Asts;

    virtual ~AST() {};
};

class FuncParam
{
  public:
    const Token* ident = nullptr;
    const Token* type = nullptr;
};

class Func : public AST
{
  public:
    ~Func() {};
    const Token* ident = nullptr;
    std::vector<FuncParam> params;
    const Token* ret = nullptr;
    std::optional<Asts> stmts = std::nullopt;
    SymbolFunc* symbol = nullptr;
};

class FuncCall : public AST
{
  public:
    ~FuncCall() {};

    const Token* ident = nullptr;
    SymbolFunc* symbolCalled = nullptr;
    Asts args;
};

class Literal : public AST
{
  public:
    ~Literal() {};

    const Token* value = nullptr;
};

void print(const AST::Ast& ast, size_t indent = 0);

void print(const AST::Asts& asts);

} // namespace Aerell