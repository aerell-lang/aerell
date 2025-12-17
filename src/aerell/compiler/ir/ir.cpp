#include <exception>
#include <format>
#include <stdexcept>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include "aerell/compiler/ir/ir.h"

namespace Aerell
{

std::unique_ptr<llvm::LLVMContext> IR::llvmContext = std::make_unique<llvm::LLVMContext>();
llvm::IRBuilder<> IR::llvmBuilder(*llvmContext);

std::unique_ptr<llvm::Module> IR::moduleTemp = nullptr;

void print(const std::unique_ptr<llvm::Module>& module) { module->print(llvm::outs(), nullptr); }

bool IR::gen(const std::vector<std::unique_ptr<AST>>& asts, std::unique_ptr<llvm::Module>& module)
{
    if(llvmContext == nullptr) llvmContext = std::make_unique<llvm::LLVMContext>();
    moduleTemp = std::make_unique<llvm::Module>("a", *llvmContext);

    auto funcExitProcessType = llvm::FunctionType::get(llvmBuilder.getVoidTy(), {llvmBuilder.getInt32Ty()}, false);
    auto funcExitProcess =
        llvm::Function::Create(funcExitProcessType, llvm::Function::ExternalLinkage, "ExitProcess", moduleTemp.get());

    auto funcPrintType =
        llvm::FunctionType::get(llvmBuilder.getVoidTy(), {llvm::PointerType::getUnqual(*llvmContext)}, false);
    llvm::Function::Create(funcPrintType, llvm::Function::ExternalLinkage, "print", moduleTemp.get());

    auto funcType = llvm::FunctionType::get(llvmBuilder.getVoidTy(), {}, false);
    auto func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "_start", moduleTemp.get());

    auto* entry = llvm::BasicBlock::Create(*llvmContext, "entry", func);
    llvmBuilder.SetInsertPoint(entry);

    for(const auto& ast : asts)
    {
        try
        {
            if(auto* funcCallCtx = dynamic_cast<FuncCall*>(ast.get())) funcCall(*funcCallCtx);
            if(auto* literalCtx = dynamic_cast<Literal*>(ast.get())) literal(*literalCtx);
        }
        catch(const std::exception& error)
        {
            llvm::errs() << error.what() << "\n";
        }
    }

    llvmBuilder.CreateCall(funcExitProcess, {llvmBuilder.getInt32(0)});

    llvmBuilder.CreateRetVoid();

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

    // Verify
    if(llvm::verifyModule(*moduleTemp, &llvm::errs())) return false;

    module = std::move(moduleTemp);

    return true;
}

llvm::Value* IR::funcCall(FuncCall& ctx)
{
    std::string_view ident{ctx.name->source->getContent().data() + ctx.name->offset, ctx.name->size};
    llvm::Function* func = moduleTemp->getFunction(ident);
    if(func == nullptr) throw std::logic_error(std::format("Use of undeclared identifier '{}'", ident));
    std::vector<llvm::Value*> arguments;
    for(const auto& arg : ctx.args)
    {
        if(auto* funcCallCtx = dynamic_cast<FuncCall*>(arg.get())) arguments.push_back(funcCall(*funcCallCtx));
        if(auto* literalCtx = dynamic_cast<Literal*>(arg.get())) arguments.push_back(literal(*literalCtx));
    }
    return llvmBuilder.CreateCall(func, arguments);
}

llvm::Value* IR::literal(Literal& ctx)
{
    std::string_view input{ctx.value->source->getContent().data() + ctx.value->offset, ctx.value->size};
    input = input.substr(1, input.size() - 2);
    std::string result;
    for(size_t i = 0; i < input.size(); ++i)
    {
        if(input[i] == '\\' && i + 1 < input.size())
        {
            switch(input[i + 1])
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
            default: result += input[i]; break;
            }
        }
        else
        {
            result += input[i];
        }
    }
    return llvmBuilder.CreateGlobalString(result);
}

} // namespace Aerell