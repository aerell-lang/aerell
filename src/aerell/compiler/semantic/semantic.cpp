#include "aerell/compiler/semantic/semantic.h"

namespace Aerell
{

bool Semantic::hasError = false;

bool Semantic::analysis(std::vector<std::unique_ptr<AST>>& asts)
{
    if(hasError) hasError = false;

    for(auto& ast : asts) expr(ast);

    return !hasError;
}

TokenType Semantic::expr(std::unique_ptr<AST>& ast)
{
    if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) return literal(*literalCtx);
    hasError = true;
    return TokenType::ILLEGAL;
}

TokenType Semantic::funcCall(FuncCall& ctx)
{
    const char* funcPrintIdent = "print";
    auto funcPrintRet = TokenType::EOFF;
    std::vector<TokenType> funcPrintArgs{TokenType::STRL};

    std::string_view funcCallIdent{ctx.name->source->getContent().data() + ctx.name->offset, ctx.name->size};
    std::vector<TokenType> funcCallArgs;
    for(auto& arg : ctx.args) funcCallArgs.push_back(expr(arg));

    if(funcCallIdent != funcPrintIdent)
    {
        ctx.name->source->printErrorMessage(ctx.name->offset, ctx.name->size, "Use of undeclared identifier");
        hasError = true;
        return TokenType::ILLEGAL;
    }

    if(funcCallArgs != funcPrintArgs)
    {
        ctx.name->source->printErrorMessage(ctx.name->offset, ctx.name->size, "Incorrect arguments to called function");
        hasError = true;
        return TokenType::ILLEGAL;
    }

    return funcPrintRet;
}

TokenType Semantic::literal(Literal& ctx) { return ctx.value->type; }

} // namespace Aerell