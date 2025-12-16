#pragma once

#include <memory>
#include <string>
#include <vector>

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

    std::string name;
    std::vector<std::unique_ptr<AST>> args;
};

class Literal : public AST
{
  public:
    ~Literal() {};

    std::string value;
};

void print(const std::unique_ptr<AST>& ast, size_t indent = 0);

void print(const std::vector<std::unique_ptr<AST>>& asts);

} // namespace Aerell