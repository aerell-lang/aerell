/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Linker/Linker.h"

#include "aerell/compiler/ir/ir.h"

namespace aerell
{

bool IR::generating(const AST::Groups& groups, Unit& unit)
{
    this->hasError = false;

    // Ctx
    unit.ctx = std::make_unique<llvm::LLVMContext>();
    this->ctx = &unit.ctx;

    // Builder
    llvm::IRBuilder<> builder(*this->ctx->get());
    this->builder = &builder;

    // Entry point
    this->entryPointModule = std::make_unique<llvm::Module>("entry_point", *unit.ctx);
    auto entryPointFuncType = llvm::FunctionType::get(this->builder->getVoidTy(), {}, false);
    auto entryPointFunc = llvm::Function::Create(
        entryPointFuncType, llvm::Function::ExternalLinkage, "_start", this->entryPointModule.get());
    auto entryPointBlock = llvm::BasicBlock::Create(*unit.ctx, "entry", entryPointFunc);

    auto end = groups.end();
    for(auto it = groups.begin(); it != groups.end(); ++it)
    {
        const AST::ChildrenWithSource& childrenWithSource = *it;
        bool isLast = (std::next(it) == end);

        if(isLast)
        {
            this->module = &this->entryPointModule;
            (*this->module)->setSourceFileName(childrenWithSource.source);
            (*this->module)->setModuleIdentifier(childrenWithSource.source);
        }
        else
        {
            this->defaultModule = std::make_unique<llvm::Module>(childrenWithSource.source, *unit.ctx);
            this->module = &this->defaultModule;
        }

        for(const AST::Ptr& ptr : childrenWithSource.children)
        {
            this->builder->SetInsertPoint(entryPointBlock);
            this->stmt(ptr);
        }

        if(isLast)
        {
            // Close entry point
            this->builder->SetInsertPoint(entryPointBlock);
            auto exitFuncType =
                llvm::FunctionType::get(this->builder->getVoidTy(), {this->builder->getInt32Ty()}, false);
            auto exitFunc = llvm::Function::Create(
                exitFuncType, llvm::Function::ExternalLinkage, "exit", this->entryPointModule.get());
            this->builder->CreateCall(exitFunc, {this->builder->getInt32(0)});
            this->builder->CreateRetVoid();
        }

        if(this->verify(*this->module)) unit.vec.push_back(std::move(*this->module));
        else if(!this->hasError)
            this->hasError = true;
    }

    return !this->hasError;
}

bool IR::verify(Ptr& ptr)
{
    // llvm return true if found error
    return !llvm::verifyModule(*ptr, &llvm::errs());
}

void IR::optimize(Ptr& ptr)
{
    llvm::PassBuilder pb;

    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cgsccam;
    llvm::ModuleAnalysisManager mam;

    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgsccam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.crossRegisterProxies(lam, fam, cgsccam, mam);

    llvm::ModulePassManager mpm = pb.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);

    mpm.run(*ptr, mam);
}

IR::Ptr IR::linking(Vec& vec)
{
    if(vec.empty()) return nullptr;

    auto mainModule = std::move(vec[std::max(0, ((int)vec.size()) - 1)]);

    llvm::Linker linker(*mainModule);

    for(auto& module : vec)
    {
        if(module == nullptr) continue;

        auto sourceFileName = module->getSourceFileName();
        if(linker.linkInModule(std::move(module)))
            llvm::errs() << "Failed to link module '" << sourceFileName << "' to '" << mainModule->getSourceFileName()
                         << "'\n";
    }

    return mainModule;
}

void IR::stmt(const AST::Ptr& ptr)
{
    if(auto* funcCtx = dynamic_cast<ASTFunc*>(ptr.get())) return func(*funcCtx);
    if(expr(ptr) != nullptr) return;
    this->hasError = true;
    llvm::errs() << "[IR] Invalid statement\n";
}

llvm::Value* IR::expr(const AST::Ptr& ptr)
{
    if(auto* funcCallCtx = dynamic_cast<ASTFuncCall*>(ptr.get())) return funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<ASTLiteral*>(ptr.get())) return literal(*literalCtx);
    return nullptr;
}

llvm::Function* IR::funcDecl(const Token& ident, const SymbolFunc& ctx)
{
    // Return
    llvm::Type* retLlvm = this->builder->getVoidTy();
    const auto& ret = ctx.getRet();
    if(ret.has_value())
    {
        if(ret.value() == DataType::I32) retLlvm = this->builder->getInt32Ty();
        else if(ret.value() == DataType::STR)
            retLlvm = this->builder->getPtrTy();
    }

    // Params
    std::vector<llvm::Type*> paramsLlvm;
    for(auto param : ctx.getParams()) switch(param)
        {
        case STR: paramsLlvm.emplace_back(this->builder->getPtrTy()); break;
        case I32: paramsLlvm.emplace_back(this->builder->getInt32Ty()); break;
        }

    // Public
    auto pubLlvm = llvm::Function::InternalLinkage;
    if(ctx.getPub()) pubLlvm = llvm::Function::ExternalLinkage;

    // Declare function
    auto fType = llvm::FunctionType::get(retLlvm, paramsLlvm, ctx.getVrdic());
    return llvm::Function::Create(fType, pubLlvm, ident.getText(), this->module->get());
}

void IR::func(ASTFunc& ctx)
{
    const auto& ident = *ctx.ident;

    auto* funcDecl = this->module->get()->getFunction(ident.getText());
    if(funcDecl == nullptr) funcDecl = this->funcDecl(ident, *ctx.symbol);

    if(!ctx.stmts.has_value()) return;

    // Block
    auto* entry = llvm::BasicBlock::Create(*this->ctx->get(), "entry", funcDecl);
    this->builder->SetInsertPoint(entry);

    // Statements
    for(const auto& stmtCtx : ctx.stmts.value())
    {
        this->builder->SetInsertPoint(entry);
        stmt(stmtCtx);
    }

    // Return void if ret is null
    if(ctx.ret == nullptr) this->builder->CreateRetVoid();
}

llvm::Value* IR::funcCall(ASTFuncCall& ctx)
{
    std::string_view ident = ctx.ident->getText();

    Ptr* previousModule = nullptr;
    if((*this->module) != this->entryPointModule)
    {
        previousModule = this->module;
        this->module = &this->entryPointModule;
    }

    llvm::Function* func = this->module->get()->getFunction(ident);

    if(func == nullptr)
    {
        if(ctx.symbolCalled == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Undefined function");
            this->hasError = true;
            if(previousModule != nullptr) this->module = previousModule;
            return nullptr;
        }

        if(this->funcDecl(*ctx.ident, *ctx.symbolCalled) == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Failed to declare function");
            this->hasError = true;
            if(previousModule != nullptr) this->module = previousModule;
            return nullptr;
        }

        func = this->module->get()->getFunction(ident);
    }

    std::vector<llvm::Value*> args;
    for(const auto& arg : ctx.args)
        if(auto value = expr(arg)) args.push_back(value);

    if(previousModule != nullptr) this->module = previousModule;

    return this->builder->CreateCall(func, args);
}

llvm::Value* IR::literal(ASTLiteral& ctx)
{
    auto value = ctx.value;
    if(value->type == TokenType::INTL)
    {
        std::string_view intl{value->source->getContent().data() + value->offset, value->size};
        return this->builder->getInt32(std::stoi(std::string(intl)));
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
        return this->builder->CreateGlobalString(result);
    }

    return nullptr;
}

void print(const IR::Ptr& ptr) { ptr->print(llvm::outs(), nullptr); }

void print(const IR::Vec& vec)
{
    for(const auto& ptr : vec)
    {
        llvm::outs() << "\n```\n";
        print(ptr);
        llvm::outs() << "```\n";
    }
}

void print(const IR::Unit& unit) { print(unit.vec); }

} // namespace aerell