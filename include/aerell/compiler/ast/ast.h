#pragma once

#include <memory>
#include <vector>

#include <aerell/compiler/token/token.h>

namespace Aerell
{

class AST
{
  public:
    virtual ~AST() {};
};

class FuncCall : public AST
{
  public:
    ~FuncCall() {};

    const Token* name;
    std::vector<std::unique_ptr<AST>> args;
};

class Literal : public AST
{
  public:
    ~Literal() {};

    const Token* value;
};

void print(const std::unique_ptr<AST>& ast, size_t indent = 0);

void print(const std::vector<std::unique_ptr<AST>>& asts);

} // namespace Aerell