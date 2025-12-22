#include "aerell/compiler/semantic/semantic.h"
#include <aerell/support/utils.h>

namespace Aerell
{

Semantic::Semantic(SymbolTable& symbolTable) : symbolTable(&symbolTable) {}

bool Semantic::analysis(const AST::Asts& asts)
{
    for(const auto& ast : asts)
        if(auto* funcCtx = dynamic_cast<Func*>(ast.get())) func(*funcCtx);

    return !this->hasError;
}

Type Semantic::expr(const std::unique_ptr<AST>& ast)
{
    if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) return literal(*literalCtx);
    return Type::VOID;
}

void Semantic::func(Func& ctx)
{
    if(ctx.stmts == std::nullopt) return;
    for(const auto& stmt : ctx.stmts.value()) expr(stmt);
}

Type Semantic::funcCall(FuncCall& ctx)
{
    const auto& indent = ctx.ident;

    auto symbolFunc = this->symbolTable->findFunc(indent->getText());
    if(symbolFunc == nullptr)
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[P] Undefined function");
        if(!this->hasError) this->hasError = true;
        return Type::VOID;
    }

    std::vector<Type> args;
    for(const auto& arg : ctx.args)
    {
        auto type = expr(arg);
        if(type == Type::VOID) continue;
        args.push_back(type);
    }

    if(symbolFunc->getParams().size() != args.size())
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] Invalid number of function arguments");
        if(!this->hasError) this->hasError = true;
        return Type::VOID;
    }

    if(args != symbolFunc->getParams())
    {
        indent->source->printErrorMessage(indent->offset, indent->size, "[S] The function argument is incorrect");
        if(!this->hasError) this->hasError = true;
        return Type::VOID;
    }

    ctx.symbolCalled = symbolFunc;

    return symbolFunc->getRet();
}

Type Semantic::literal(Literal& ctx)
{
    if(ctx.value->type == TokenType::INTL) return Type::I32;
    if(ctx.value->type == TokenType::STRL) return Type::STR;
    else
        return Type::VOID;
}

} // namespace Aerell