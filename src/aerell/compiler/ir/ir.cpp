#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include "aerell/compiler/ir/ir.h"

namespace Aerell
{

IR::Context& IR::getContext() { return this->llvmContext; }

bool IR::generating(const char* sourceFileName, const AST::Asts& asts, Module& module)
{
    if(hasError) hasError = false;
    if(llvmContext == nullptr) llvmContext = std::make_unique<llvm::LLVMContext>();
    if(moduleTemp == nullptr) moduleTemp = std::make_unique<llvm::Module>(sourceFileName, *llvmContext);

    for(const auto& ast : asts)
        if(auto* funcCtx = dynamic_cast<Func*>(ast.get())) func(*funcCtx);

    if(hasError) return false;

    // Verify
    if(llvm::verifyModule(*moduleTemp, &llvm::errs())) return false;

    // Optimize
    llvm::PassBuilder passBuilder;

    llvm::LoopAnalysisManager loopAM;
    llvm::FunctionAnalysisManager funcAM;
    llvm::CGSCCAnalysisManager cgsccAM;
    llvm::ModuleAnalysisManager moduleAM;

    passBuilder.registerModuleAnalyses(moduleAM);
    passBuilder.registerCGSCCAnalyses(cgsccAM);
    passBuilder.registerFunctionAnalyses(funcAM);
    passBuilder.registerLoopAnalyses(loopAM);
    passBuilder.crossRegisterProxies(loopAM, funcAM, cgsccAM, moduleAM);

    llvm::ModulePassManager modulePM = passBuilder.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);

    modulePM.run(*moduleTemp, moduleAM);

    module = std::move(moduleTemp);

    return !hasError;
}

llvm::Value* IR::expr(const AST::Ast& ast)
{
    if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) return literal(*literalCtx);
    return nullptr;
}

llvm::Function* IR::funcDecl(const Token& ident, SymbolFunc& ctx)
{
    // Return
    llvm::Type* ret = llvmBuilder.getVoidTy();
    switch(ctx.getRet())
    {
    case Type::VOID: ret = llvmBuilder.getVoidTy(); break;
    case Type::I32: ret = llvmBuilder.getInt32Ty(); break;
    case Type::STR: ret = llvmBuilder.getPtrTy(); break;
    }

    // Params
    std::vector<llvm::Type*> params;
    for(auto param : ctx.getParams()) switch(param)
        {
        case VOID:
            ident.source->printErrorMessage(ident.offset, ident.size, "[IR] Invalid param type");
            this->hasError = true;
            break;
        case STR: params.emplace_back(llvmBuilder.getPtrTy()); break;
        case I32: params.emplace_back(llvmBuilder.getInt32Ty()); break;
        }

    // Public
    auto pub = llvm::Function::InternalLinkage;
    if(ctx.getPub()) pub = llvm::Function::ExternalLinkage;

    // Declare function
    auto fType = llvm::FunctionType::get(ret, params, ctx.getVrdic());
    return llvm::Function::Create(fType, pub, ident.getText(), moduleTemp.get());
}

void IR::func(Func& ctx)
{
    auto* funcDecl = this->funcDecl(*ctx.ident, *ctx.symbol);

    if(!ctx.stmts.has_value()) return;

    // Block
    auto* entry = llvm::BasicBlock::Create(*llvmContext, "entry", funcDecl);
    llvmBuilder.SetInsertPoint(entry);

    // Statements
    for(const auto& stmt : ctx.stmts.value()) expr(stmt);

    // Return void if ret is null
    if(ctx.ret == nullptr) llvmBuilder.CreateRetVoid();
}

llvm::Value* IR::funcCall(FuncCall& ctx)
{
    std::string_view ident = ctx.ident->getText();
    llvm::Function* func = moduleTemp->getFunction(ident);

    if(func == nullptr)
    {
        if(ctx.symbolCalled == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Undefined function");
            this->hasError = true;
            return nullptr;
        }

        if(this->funcDecl(*ctx.ident, *ctx.symbolCalled) == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Failed to declare function");
            this->hasError = true;
            return nullptr;
        }

        func = moduleTemp->getFunction(ident);
    }

    std::vector<llvm::Value*> args;
    for(const auto& arg : ctx.args)
        if(auto value = expr(arg)) args.push_back(value);

    return llvmBuilder.CreateCall(func, args);
}

llvm::Value* IR::literal(Literal& ctx)
{
    auto value = ctx.value;
    if(value->type == TokenType::INTL)
    {
        std::string_view intl{value->source->getContent().data() + value->offset, value->size};
        return llvmBuilder.getInt32(std::stoi(std::string(intl)));
    }

    if(value->type == TokenType::STRL)
    {
        std::string_view strl{value->source->getContent().data() + value->offset, value->size};
        strl = strl.substr(1, strl.size() - 2);
        std::string result;
        for(size_t i = 0; i < strl.size(); ++i)
        {
            if(strl[i] == '\\' && i + 1 < strl.size())
            {
                switch(strl[i + 1])
                {
                case 'n':
                    result += '\n';
                    i++;
                    break;
                case 't':
                    result += '\t';
                    i++;
                    break;
                case '\\':
                    result += '\\';
                    i++;
                    break;
                case '"':
                    result += '"';
                    i++;
                    break;
                default: result += strl[i]; break;
                }
            }
            else
            {
                result += strl[i];
            }
        }
        return llvmBuilder.CreateGlobalString(result);
    }

    return nullptr;
}

void print(const IR::Module& module) { module->print(llvm::outs(), nullptr); }

} // namespace Aerell