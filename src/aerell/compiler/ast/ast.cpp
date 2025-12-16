#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/ast/ast.h"

namespace Aerell
{

void print(const std::unique_ptr<AST>& ast, size_t indent)
{
    if(auto* funcCall = dynamic_cast<FuncCall*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "FuncCall\n";
        llvm::outs() << std::string(indent, ' ') << " name: " << funcCall->name << "\n";
        if(funcCall->args.empty()) return;
        llvm::outs() << std::string(indent, ' ') << " args:\n";
        for(const auto& arg : funcCall->args) print(arg, indent + 2);
    }
    if(auto* literal = dynamic_cast<Literal*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "Literal\n";
        llvm::outs() << std::string(indent, ' ') << " value: " << literal->value << "\n";
    }
}

void print(const std::vector<std::unique_ptr<AST>>& asts)
{
    for(const auto& ast : asts) print(ast);
}

} // namespace Aerell