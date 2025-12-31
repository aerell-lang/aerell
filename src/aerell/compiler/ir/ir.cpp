/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <format>
#include <variant>

#include "aerell/compiler/ir/ir.h"
#include "aerell/compiler/ir/ir_val.h"
#include "aerell/compiler/ir/ir_instruct.h"
#include "aerell/compiler/ir/ir_i32.h"
#include "aerell/compiler/ir/ir_str.h"
#include "aerell/compiler/ir/ir_func_call.h"

namespace aerell
{

bool IR::generating(const AST::Groups& groups, IRMod::Vec& vec)
{
    this->hasError = false;

    auto startMod = std::make_unique<IRMod>("start");

    IRBlock startFuncBlock;

    size_t i = 0;
    for(const AST::ChildrenWithSource& childrenWithSource : groups)
    {
        this->mod = std::make_unique<IRMod>(childrenWithSource.source);

        IRBlock initFuncBlock;

        for(const AST::Ptr& ptr : childrenWithSource.children)
        {
            this->block = &initFuncBlock;
            this->stmt(ptr);
        }

        i++;
        if(this->hasError) continue;

        IRFunc initFunc(true, IRTypes{}, false, std::nullopt);

        IRFunc initFuncDeclInStartMod(true, IRTypes{}, false, std::nullopt);

        auto initFuncIdent = std::format("init{}", i);
        if(startMod->addFunc(initFuncIdent, std::move(initFuncDeclInStartMod)) == nullptr)
        {
            this->hasError = true;
            errs() << "Failed to create declaration function of " << initFuncIdent << " in start module.\n";
            continue;
        }

        auto initFuncBlockLabel = "entry";
        if(initFunc.addBlock(initFuncBlockLabel, std::move(initFuncBlock)) == nullptr)
        {
            this->hasError = true;
            errs() << "Failed to create " << initFuncBlockLabel << " label for " << initFuncIdent << " function.\n";
            continue;
        }

        if(this->mod->addFunc(initFuncIdent, std::move(initFunc)) == nullptr)
        {
            this->hasError = true;
            errs() << "Failed to create declaration function of " << initFuncIdent << " in a module of "
                   << childrenWithSource.source << ".\n";
            continue;
        }

        startFuncBlock.addInstruct(std::make_unique<IRFuncCall>(std::move(initFuncIdent), IRVal::Vec{}));

        vec.emplace_back(std::move(this->mod));
    }

    IRFunc exitFunc(true, IRTypes{IRType::I32}, false, std::nullopt);

    auto exitFuncIdent = "exit";
    if(startMod->addFunc(exitFuncIdent, std::move(exitFunc)) == nullptr)
    {
        errs() << "Failed to create declaration function of " << exitFuncIdent << " in start module.\n";
        return false;
    }

    IRVal::Vec extFuncArgs;
    extFuncArgs.emplace_back(std::make_unique<IRI32>(0));
    startFuncBlock.addInstruct(std::make_unique<IRFuncCall>(exitFuncIdent, std::move(extFuncArgs)));

    IRFunc startFunc(true, IRTypes{}, false, std::nullopt);

    auto startFuncIdent = "_start";
    auto startFuncBlockLabel = "entry";

    if(startFunc.addBlock(startFuncBlockLabel, std::move(startFuncBlock)) == nullptr)
    {
        errs() << "Failed to create " << startFuncBlockLabel << " label for " << startFuncIdent << " function.\n";
        return false;
    }

    if(startMod->addFunc(startFuncIdent, std::move(startFunc)) == nullptr)
    {
        errs() << "Failed to create declaration function of " << startFuncIdent << " in start module.\n";
        return false;
    }

    vec.emplace_back(std::move(startMod));

    return !this->hasError;
}

void IR::stmt(const AST::Ptr& ptr)
{
    if(auto* funcCtx = dynamic_cast<ASTFunc*>(ptr.get())) return this->func(*funcCtx);
    if(IRVal::Ptr exprCtx = expr(ptr); exprCtx != nullptr)
        if(dynamic_cast<IRInstruct*>(exprCtx.get()))
        {
            this->block->addInstruct(IRInstruct::Ptr(static_cast<IRInstruct*>(exprCtx.release())));
            return;
        }
    this->hasError = true;
    errs() << "[IR] a Invalid statement\n";
}

IRVal::Ptr IR::expr(const AST::Ptr& ptr)
{
    if(auto* funcCallCtx = dynamic_cast<ASTFuncCall*>(ptr.get())) return this->funcCall(*funcCallCtx);
    if(auto* literalCtx = dynamic_cast<ASTLiteral*>(ptr.get())) return this->literal(*literalCtx);
    return nullptr;
}

void IR::func(ASTFunc& ctx)
{
    const auto& ident = (std::string)(*ctx.ident).getText();

    std::variant<IRFunc*, IRFunc> funcVariant = this->mod->getFunc(ident);
    IRFunc* funcTemp = nullptr;
    if(std::holds_alternative<IRFunc*>(funcVariant) && std::get<IRFunc*>(funcVariant) == nullptr)
    {
        const auto& symbol = ctx.symbol;
        funcVariant = IRFunc(symbol->getPub(), symbol->getParams(), symbol->getVrdic(), symbol->getRet());
        funcTemp = &std::get<IRFunc>(funcVariant);
    }

    if(ctx.stmts.has_value())
    {
        IRBlock block;

        // Statements
        for(const auto& stmtCtx : ctx.stmts.value())
        {
            this->block = &block;
            stmt(stmtCtx);
        }

        auto blockLabel = "entry";
        if(funcTemp->addBlock(blockLabel, std::move(block)) == nullptr)
        {
            this->hasError = true;
            errs() << "Failed to create " << blockLabel << " label for " << ident << " function.\n";
            return;
        }
    }

    if(std::holds_alternative<IRFunc>(funcVariant)) this->mod->addFunc(ident, std::move(*funcTemp));
}

IRVal::Ptr IR::funcCall(ASTFuncCall& ctx)
{
    const auto& ident = (std::string)ctx.ident->getText();

    IRFunc* func = this->mod->getFunc(ident);

    if(func == nullptr)
    {
        if(ctx.symbolCalled == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Undefined function");
            this->hasError = true;
            return nullptr;
        }

        const auto& symbol = ctx.symbolCalled;
        if(this->mod->addFunc(
               ident, IRFunc(symbol->getPub(), symbol->getParams(), symbol->getVrdic(), symbol->getRet())) == nullptr)
        {
            ctx.ident->source->printErrorMessage(ctx.ident->offset, ctx.ident->size, "[IR] Failed to declare function");
            this->hasError = true;
            return nullptr;
        }

        func = this->mod->getFunc(ident);
    }

    IRVal::Vec args;
    for(const auto& arg : ctx.args)
        if(auto value = expr(arg)) args.push_back(std::move(value));

    return std::make_unique<IRFuncCall>(ident, std::move(args));
}

IRVal::Ptr IR::literal(ASTLiteral& ctx)
{
    auto value = ctx.value;
    if(value->type == TokenType::INTL)
    {
        std::string_view intl{value->source->getContent().data() + value->offset, value->size};
        return std::make_unique<IRI32>(std::stoi(std::string(intl)));
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
        return std::make_unique<IRStr>(std::move(result));
    }

    return nullptr;
}

bool IR::generating(const IRMod::Vec& vec, IRllvm::Unit& unit)
{
    this->hasError = false;

    unit.ctx = std::make_unique<llvm::LLVMContext>();

    llvm::IRBuilder<> builder(*unit.ctx);

    for(const IRMod::Ptr& ptr : vec)
        if(IRllvm::Ptr llvmPtr = ptr->toLlvm(unit.ctx, builder)) unit.vec.emplace_back(std::move(llvmPtr));
        else
            this->hasError = true;

    return !this->hasError;
}

} // namespace aerell