/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <memory>

#include <llvm/Support/raw_ostream.h>

#include <aerell/support/utils.h>
#include <aerell/compiler/compiler.h>
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

namespace Aerell
{

Compiler::Tokens Compiler::lexing(Source* source)
{
    // Tokenize
    Tokens cTokens;
    size_t lastImportIndex = 0;

    auto mainTokens = this->lexer.lexing(source);

    for(auto token : mainTokens)
    {
        if(token.type != TokenType::STRL) break;
        lastImportIndex++;

        std::string errorMessage;
        llvm::raw_string_ostream os(errorMessage);
        int status = sourceManager.import(std::string(token.getText().substr(1, token.size - 2)).c_str(), os);
        if(status == 0)
        {
            errorMessage = {"[I] " + errorMessage};
            token.source->printErrorMessage(token.offset, token.size, errorMessage.c_str());
            continue;
        }
        else if(status == 2)
            continue;

        for(auto& token : lexing(sourceManager.getLastSource())) cTokens.push_back(std::move(token));
    }

    mainTokens = {
        std::make_move_iterator(mainTokens.begin() + lastImportIndex), std::make_move_iterator(mainTokens.end())};
    if(mainTokens.size() != 1 && mainTokens[0].type != TokenType::EOFF) cTokens.emplace_back(mainTokens);

    return cTokens;
}

Compiler::Tokens Compiler::lexing(const char* filePath)
{
    Compiler::Tokens mainTokens;

    std::vector<std::pair<const char*, bool>> autoImportSources{
        {"std/std", false},
        {filePath, true},
    };

    for(const auto& autoImportSource : autoImportSources)
    {
        // Import file
        int status = this->sourceManager.import(autoImportSource.first, llvm::errs(), autoImportSource.second);
        if(status == 0)
        {
            llvm::errs() << '\n';
            continue;
        }
        else if(status == 2)
            continue;

        // Lexing
        for(auto& tokens : this->lexing(this->sourceManager.getLastSource())) mainTokens.push_back(std::move(tokens));
    }

    return mainTokens;
}

bool Compiler::parsing(const Tokens& cTokens, Asts& cAsts)
{
    bool hasError = false;

    for(const Lexer::Tokens& tokens : cTokens)
    {
        AST::Asts asts;
        if(!this->parser.parsing(tokens, asts))
        {
            if(!hasError) hasError = true;
            continue;
        }

        cAsts.push_back(std::move(asts));
    }

    return !hasError;
}

bool Compiler::analysis(const Asts& cAsts)
{
    bool hasError = false;

    for(const AST::Asts& asts : cAsts)
        if(!this->semantic.analysis(asts))
            if(!hasError) hasError = true;

    return !hasError;
}

bool Compiler::generating(const Tokens& tokens, const Asts& cAsts, IR::Modules& modules)
{
    bool hasError = false;

    size_t index = 0;
    for(const AST::Asts& asts : cAsts)
    {
        auto sourceFileName = tokens[index].front().source->getPath().c_str();
        index++;
        IR::Module module = nullptr;
        if(!this->ir.generating(sourceFileName, asts, module))
        {
            if(!hasError) hasError = true;
            continue;
        }

        modules.push_back(std::move(module));
    }

    auto startModule = this->ir.getStartModule();
    if(startModule == nullptr && !hasError) hasError = true;
    if(!this->ir.linking(startModule, modules[std::max(0, ((int)modules.size()) - 1)]) && !hasError) hasError = true;

    return !hasError;
}

IR::Module Compiler::linking(IR::Modules& modules) { return this->ir.linking(modules); }

void Compiler::optimize(IR::Modules& modules)
{
    IR::Modules optimizeModules;
    for(auto& module : modules)
    {
        this->optimize(module);
        if(module == nullptr) continue;
        optimizeModules.push_back(std::move(module));
    }
    modules = std::move(optimizeModules);
}

void Compiler::optimize(IR::Module& module)
{
    this->ir.optimize(module);
    if(module->empty()) module = nullptr;
}

bool Compiler::compile(IR::Modules& modules, std::vector<std::string>& outputs)
{
    if(modules.empty()) return false;

    // Code Gen
    return std::all_of(modules.begin(), modules.end(), [&](auto& module) {
        auto output = this->compile(module);
        if(!output.has_value()) return false;
        outputs.push_back(output.value());
        return true;
    });
}

std::optional<std::string> Compiler::compile(IR::Module& module)
{
    const auto& filePath = module->getSourceFileName();
    if(!Aerell::CodeGen::obj(filePath.c_str(), module)) return std::nullopt;
    return filePath;
}

void print(const Compiler::Tokens& cTokens)
{
    for(const Lexer::Tokens& tokens : cTokens) print(tokens);
}

void print(const Compiler::Asts& cAsts)
{
    for(const AST::Asts& asts : cAsts) print(asts);
}

void print(const IR::Modules& modules)
{
    for(const IR::Module& module : modules)
    {
        llvm::outs() << "\n```\n";
        print(module);
        llvm::outs() << "```\n";
    }
}

} // namespace Aerell