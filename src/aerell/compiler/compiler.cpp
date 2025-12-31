/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <memory>

#include <aerell/support/utils.h>
#include <aerell/compiler/compiler.h>
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

namespace aerell
{

const SymbolTable& Compiler::getSymbolTable() const { return this->symbolTable; }

bool Compiler::lexing(Source* source, Token::Vecs& vecs)
{
    bool hasError = false;

    size_t lastImportIndex = 0;

    auto mainTokens = this->lexer.lexing(source);

    for(auto token : mainTokens)
    {
        if(token.type != TokenType::STRL) break;
        lastImportIndex++;

        std::string errorMessage;
        OSStream os(errorMessage);
        int status = sourceManager.import(std::string(token.getText().substr(1, token.size - 2)).c_str(), os);
        if(status == 0)
        {
            errorMessage = {"[I] " + errorMessage};
            token.source->printErrorMessage(token.offset, token.size, errorMessage.c_str());
            if(!hasError) hasError = true;
            continue;
        }
        else if(status == 2)
            continue;

        Token::Vecs vecsTemp;
        if(!this->lexing(this->sourceManager.getLastSource(), vecsTemp))
        {
            if(!hasError) hasError = true;
            continue;
        }

        for(auto& tokens : vecsTemp) vecs.push_back(std::move(tokens));
    }

    mainTokens = {
        std::make_move_iterator(mainTokens.begin() + lastImportIndex), std::make_move_iterator(mainTokens.end())};
    if(mainTokens.size() != 1 && mainTokens[0].type != TokenType::EOFF) vecs.emplace_back(mainTokens);

    return !hasError;
}

bool Compiler::lexing(const char* filePath, Token::Vecs& vecs)
{
    bool hasError = false;

    std::vector<std::pair<const char*, bool>> autoImportSources{
        {"std/std", false},
        {filePath, true},
    };

    for(const auto& autoImportSource : autoImportSources)
    {
        // Import file
        int status = this->sourceManager.import(autoImportSource.first, errs(), autoImportSource.second);
        if(status == 0)
        {
            errs() << '\n';
            if(!hasError) hasError = true;
            continue;
        }
        else if(status == 2)
            continue;

        Token::Vecs vecsTemp;
        if(!this->lexing(this->sourceManager.getLastSource(), vecsTemp))
        {
            if(!hasError) hasError = true;
            continue;
        }

        // Lexing
        for(auto& tokens : vecsTemp) vecs.push_back(std::move(tokens));
    }

    return !hasError;
}

bool Compiler::parsing(const Token::Vecs& vecs, AST::Groups& groups)
{
    bool hasError = false;

    for(const Token::Vec& vec : vecs)
    {
        AST::ChildrenWithSource childrenWithSource;
        if(!this->parser.parsing(vec, childrenWithSource))
        {
            if(!hasError) hasError = true;
            continue;
        }

        groups.push_back(std::move(childrenWithSource));
    }

    return !hasError;
}

bool Compiler::analysis(const AST::Groups& groups)
{
    bool hasError = false;

    for(const AST::ChildrenWithSource& childrenWithSource : groups)
        if(!this->semantic.analysis(childrenWithSource))
            if(!hasError) hasError = true;

    return !hasError;
}

bool Compiler::generating(const AST::Groups& groups, IRMod::Vec& vec) { return this->ir.generating(groups, vec); }

bool Compiler::generating(const IRMod::Vec& vec, IRllvm::Unit& unit) { return this->ir.generating(vec, unit); }

IRllvm::Ptr Compiler::linking(IRllvm::Vec& vec) { return IRllvm::linking(vec); }

bool Compiler::linking(IRllvm::Unit& unit)
{
    auto ptr = this->linking(unit.vec);
    if(ptr == nullptr) return false;
    unit.vec.clear();
    unit.vec.push_back(std::move(ptr));
    return true;
}

void Compiler::optimize(IRllvm::Ptr& ptr)
{
    IRllvm::optimize(ptr);
    if(ptr->empty()) ptr = nullptr;
}

void Compiler::optimize(IRllvm::Vec& vec)
{
    IRllvm::Vec optimizeVec;
    for(IRllvm::Ptr& ptr : vec)
    {
        this->optimize(ptr);
        if(ptr == nullptr) continue;
        optimizeVec.push_back(std::move(ptr));
    }
    vec = std::move(optimizeVec);
}

void Compiler::optimize(IRllvm::Unit& unit) { this->optimize(unit.vec); }

std::optional<std::string> Compiler::compile(IRllvm::Ptr& ptr)
{
    const auto& filePath = ptr->getSourceFileName();
    if(!aerell::CodeGen::obj(filePath.c_str(), ptr)) return std::nullopt;
    return filePath;
}

bool Compiler::compile(IRllvm::Vec& vec, std::vector<std::string>& outputs)
{
    if(vec.empty()) return false;

    // Code Gen
    return std::all_of(vec.begin(), vec.end(), [&](auto& module) {
        auto output = this->compile(module);
        if(!output.has_value()) return false;
        outputs.push_back(output.value());
        return true;
    });
}

bool Compiler::compile(IRllvm::Unit& unit, std::vector<std::string>& outputs)
{
    return this->compile(unit.vec, outputs);
}

} // namespace aerell