#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/ast/ast.h"

namespace Aerell
{

void print(const std::unique_ptr<AST>& ast, size_t indent)
{
    if(auto* func = dynamic_cast<Func*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "Func\n";
        llvm::outs() << std::string(indent, ' ') << " pub: " << func->symbol->getPub() << "\n";
        llvm::outs()
            << std::string(indent, ' ') << " ident: "
            << std::string_view{func->ident->source->getContent().data() + func->ident->offset, func->ident->size}
            << "\n";
        if(!func->params.empty())
        {
            llvm::outs() << std::string(indent, ' ') << " params:\n";
            for(auto param : func->params)
            {
                llvm::outs() << std::string(indent + 1, ' ') << " FuncParam:\n";
                llvm::outs()
                    << std::string(indent + 2, ' ') << " ident: "
                    << std::
                           string_view{param.ident->source->getContent().data() + param.ident->offset, param.ident->size}
                    << "\n";
                llvm::outs()
                    << std::string(indent + 2, ' ') << " type: "
                    << std::string_view{param.type->source->getContent().data() + param.type->offset, param.type->size}
                    << "\n";
            }
        }
        llvm::outs() << std::string(indent, ' ') << " vrdic: " << func->symbol->getVrdic() << "\n";
        if(func->ret != nullptr)
            llvm::outs()
                << std::string(indent, ' ') << " ret: "
                << std::string_view{func->ret->source->getContent().data() + func->ret->offset, func->ret->size}
                << "\n";
        if(func->stmts.has_value())
        {
            llvm::outs() << std::string(indent, ' ') << " stmts:\n";
            for(const auto& stmt : func->stmts.value()) print(stmt, indent + 2);
        }
    }
    if(auto* funcCall = dynamic_cast<FuncCall*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "FuncCall\n";
        llvm::outs()
            << std::string(indent, ' ') << " name: "
            << std::
                   string_view{funcCall->ident->source->getContent().data() + funcCall->ident->offset, funcCall->ident->size}
            << "\n";
        if(funcCall->args.empty()) return;
        llvm::outs() << std::string(indent, ' ') << " args:\n";
        for(const auto& arg : funcCall->args) print(arg, indent + 2);
    }
    if(auto* literal = dynamic_cast<Literal*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "Literal\n";
        llvm::outs()
            << std::string(indent, ' ') << " value: "
            << std::
                   string_view{literal->value->source->getContent().data() + literal->value->offset, literal->value->size}
            << "\n";
    }
}

void print(const std::vector<std::unique_ptr<AST>>& asts)
{
    for(const auto& ast : asts) print(ast);
}

} // namespace Aerell