#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Linker/Linker.h"

#include "aerell/compiler/ir/ir.h"

namespace Aerell
{

IR::Context& IR::getContext() { return this->llvmContext; }

bool IR::verify(Module& module)
{
    // Verify
    return !llvm::verifyModule(*module, &llvm::errs());
}

void IR::optimize(Module& module)
{
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

    modulePM.run(*module, moduleAM);
}

IR::Module IR::getStartModule()
{
    llvmBuilder.SetInsertPoint(this->startFuncEntry);

    auto exitFuncType = llvm::FunctionType::get(llvmBuilder.getVoidTy(), {llvmBuilder.getInt32Ty()}, false);
    auto exitFunc =
        llvm::Function::Create(exitFuncType, llvm::Function::ExternalLinkage, "exit", this->moduleStart.get());

    llvmBuilder.CreateCall(exitFunc, {llvmBuilder.getInt32(0)});

    llvmBuilder.CreateRetVoid();

    // Verify
    if(!this->verify(this->moduleStart)) return nullptr;

    return std::move(this->moduleStart);
}

bool IR::linking(Module& module, Module& moduleDest)
{
    llvm::Linker linker(*moduleDest);

    auto sourceFileName = module->getSourceFileName();
    if(linker.linkInModule(std::move(module)))
    {
        llvm::errs() << "Failed to link module '" << sourceFileName << "' to '" << moduleDest->getSourceFileName()
                     << "'\n";
        return false;
    }

    return true;
}

IR::Module IR::linking(Modules& modules)
{
    if(modules.empty()) return nullptr;

    auto mainModule = std::move(modules[std::max(0, ((int)modules.size()) - 1)]);

    llvm::Linker linker(*mainModule);

    for(auto& module : modules)
    {
        if(module == nullptr) continue;

        auto sourceFileName = module->getSourceFileName();
        if(linker.linkInModule(std::move(module)))
            llvm::errs() << "Failed to link module '" << sourceFileName << "' to '" << mainModule->getSourceFileName()
                         << "'\n";
    }

    return mainModule;
}

bool IR::generating(const char* sourceFileName, const AST::Asts& asts, Module& module)
{
    if(this->hasError) this->hasError = false;
    if(this->llvmContext == nullptr) this->llvmContext = std::make_unique<llvm::LLVMContext>();
    if(this->moduleTemp == nullptr) this->moduleTemp = std::make_unique<llvm::Module>(sourceFileName, *llvmContext);
    if(this->moduleStart == nullptr)
    {
        this->moduleStart = std::make_unique<llvm::Module>("start", *llvmContext);
        auto startFuncType = llvm::FunctionType::get(llvmBuilder.getVoidTy(), {}, false);
        auto startFunc =
            llvm::Function::Create(startFuncType, llvm::Function::ExternalLinkage, "_start", this->moduleStart.get());
        this->startFuncEntry = llvm::BasicBlock::Create(*llvmContext, "entry", startFunc);
    }

    for(const auto& ast : asts)
    {
        llvmBuilder.SetInsertPoint(this->startFuncEntry);
        this->stmt(ast, this->moduleStart);
    }

    // HasError & Verify
    if(hasError || !this->verify(this->moduleTemp)) return false;

    module = std::move(this->moduleTemp);

    return !hasError;
}

void IR::stmt(const AST::Ast& ast, const Module& module)
{
    if(auto* funcCtx = dynamic_cast<Func*>(ast.get())) return func(*funcCtx);
    if(expr(ast, module)) return;
    this->hasError = true;
    llvm::errs() << "[IR] Invalid statement\n";
}

llvm::Value* IR::expr(const AST::Ast& ast, const Module& module)
{
    if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) return funcCall(*funcCallCtx, module);
    if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) return literal(*literalCtx);
    return nullptr;
}

llvm::Function* IR::funcDecl(const Token& ident, SymbolFunc& ctx, const Module& module)
{
    // Return
    llvm::Type* retLlvm = llvmBuilder.getVoidTy();
    switch(ctx.getRet())
    {
    case Type::VOID: retLlvm = llvmBuilder.getVoidTy(); break;
    case Type::I32: retLlvm = llvmBuilder.getInt32Ty(); break;
    case Type::STR: retLlvm = llvmBuilder.getPtrTy(); break;
    }

    // Params
    std::vector<llvm::Type*> paramsLlvm;
    for(auto param : ctx.getParams()) switch(param)
        {
        case VOID:
            ident.source->printErrorMessage(ident.offset, ident.size, "[IR] Invalid param type");
            this->hasError = true;
            break;
        case STR: paramsLlvm.emplace_back(llvmBuilder.getPtrTy()); break;
        case I32: paramsLlvm.emplace_back(llvmBuilder.getInt32Ty()); break;
        }

    // Public
    auto pubLlvm = llvm::Function::InternalLinkage;
    if(ctx.getPub()) pubLlvm = llvm::Function::ExternalLinkage;

    // Declare function
    auto fType = llvm::FunctionType::get(retLlvm, paramsLlvm, ctx.getVrdic());
    return llvm::Function::Create(fType, pubLlvm, ident.getText(), module.get());
}

void IR::func(Func& ctx)
{
    const auto& ident = *ctx.ident;

    auto* funcDecl = this->moduleTemp->getFunction(ident.getText());
    if(funcDecl == nullptr) funcDecl = this->funcDecl(ident, *ctx.symbol, this->moduleTemp);

    if(!ctx.stmts.has_value()) return;

    // Block
    auto* entry = llvm::BasicBlock::Create(*llvmContext, "entry", funcDecl);
    llvmBuilder.SetInsertPoint(entry);

    // Statements
    for(const auto& stmtCtx : ctx.stmts.value()) stmt(stmtCtx, this->moduleTemp);

    // Return void if ret is null
    if(ctx.ret == nullptr) llvmBuilder.CreateRetVoid();
}

llvm::Value* IR::funcCall(FuncCall& ctx, const Module& module)
{
    std::string_view ident = ctx.ident->getText();
    llvm::Function* func = module->getFunction(ident);

    if(func == nullptr)
    {
        if(ctx.symbolCalled == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Undefined function");
            this->hasError = true;
            return nullptr;
        }

        if(this->funcDecl(*ctx.ident, *ctx.symbolCalled, module) == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Failed to declare function");
            this->hasError = true;
            return nullptr;
        }

        func = module->getFunction(ident);
    }

    std::vector<llvm::Value*> args;
    for(const auto& arg : ctx.args)
        if(auto value = expr(arg, module)) args.push_back(value);

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