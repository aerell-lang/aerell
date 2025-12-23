/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/ast/ast.h"

namespace Aerell
{

void print(const AST::Ast& ast, size_t indent)
{
    // Func
    if(auto* func = dynamic_cast<Func*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "Func\n";
        llvm::outs() << std::string(indent, ' ') << " ident: " << func->ident->getText() << "\n";
        if(!func->params.empty())
        {
            llvm::outs() << std::string(indent, ' ') << " params:\n";
            for(auto param : func->params)
            {
                llvm::outs() << std::string(indent + 1, ' ') << " FuncParam\n";
                llvm::outs() << std::string(indent + 2, ' ') << " ident: " << param.ident->getText() << "\n";
                llvm::outs() << std::string(indent + 2, ' ') << " type: " << param.type->getText() << "\n";
            }
        }
        if(func->ret != nullptr) llvm::outs() << std::string(indent, ' ') << " ret: " << func->ret->getText() << "\n";
        if(func->stmts.has_value())
        {
            llvm::outs() << std::string(indent, ' ') << " stmts:\n";
            for(const auto& stmt : func->stmts.value()) print(stmt, indent + 2);
        }
    }

    // FuncCall
    if(auto* funcCall = dynamic_cast<FuncCall*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "FuncCall\n";
        llvm::outs() << std::string(indent, ' ') << " name: " << funcCall->ident->getText() << "\n";
        if(funcCall->args.empty()) return;
        llvm::outs() << std::string(indent, ' ') << " args:\n";
        for(const auto& arg : funcCall->args) print(arg, indent + 2);
    }

    // Literal
    if(auto* literal = dynamic_cast<Literal*>(ast.get()))
    {
        llvm::outs() << std::string(indent, ' ') << "Literal\n";
        llvm::outs() << std::string(indent, ' ') << " value: " << literal->value->getText() << "\n";
    }
}

void print(const AST::Asts& asts)
{
    const char* path = nullptr;
    for(const auto& ast : asts)
    {
        auto sourcePath = ast->path;
        if(path != sourcePath)
        {
            path = sourcePath;
            llvm::outs() << '\n' << path << ":\n";
        }
        print(ast);
    }
}

} // namespace Aerell