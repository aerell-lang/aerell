/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/TargetSelect.h>

#include "aerell/cli/cli.h"
#include "aerell/linker/linker.h"
#include "aerell/compiler/compiler.h"
#include "aerell/compiler/symbol/symbol_print.h"

int main(int argc, char* argv[])
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetDisassembler();

    aerell::CLI cli;
    aerell::Compiler compiler;
    aerell::Linker linker;

    if(argc == 1)
    {
        cli.printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "help") == 0)
    {
        cli.printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "version") == 0)
    {
        cli.printVersion();
        return EXIT_SUCCESS;
    }

    bool isLex = false;
    bool isParse = false;
    bool isAnalyze = false;
    bool isGenerate = false;
    bool isRun = false;
    bool isCompile = false;
    bool isBuild = false;
    if(argc > 1)
    {
        isLex = std::strcmp(argv[1], "lex") == 0;
        isParse = std::strcmp(argv[1], "parse") == 0;
        isAnalyze = std::strcmp(argv[1], "analyze") == 0;
        isGenerate = std::strcmp(argv[1], "generate") == 0;
        isRun = std::strcmp(argv[1], "run") == 0;
        isCompile = std::strcmp(argv[1], "compile") == 0;
        isBuild = std::strcmp(argv[1], "build") == 0;
    }

    if(argc == 3 && (isLex || isParse || isAnalyze || isGenerate || isRun || isCompile || isBuild))
    {
        const auto& filePath = argv[2];

        // Lexing
        aerell::Token::Vecs vecs;
        if(!compiler.lexing(filePath, vecs)) return EXIT_FAILURE;
        if(isLex)
        {
            aerell::print(vecs);
            llvm::outs() << "\nLexing finished.";
            return EXIT_SUCCESS;
        }

        // Parsing
        aerell::AST::Groups groups;
        if(!compiler.parsing(vecs, groups)) return EXIT_FAILURE;
        if(isParse)
        {
            llvm::outs() << groups << "\nParsing completed.";
            return EXIT_SUCCESS;
        }

        // Analysis
        if(!compiler.analysis(groups)) return EXIT_FAILURE;
        if(isAnalyze)
        {
            aerell::print(compiler.getSymbolTable());
            llvm::outs() << "\nAnalysis completed.";
            return EXIT_SUCCESS;
        }

        // IR Gen
        aerell::IR::Unit unit;
        if(!compiler.generating(groups, unit)) return EXIT_FAILURE;
        if(!compiler.linking(unit)) return EXIT_FAILURE;
        compiler.optimize(unit);

        if(isGenerate)
        {
            aerell::print(unit);
            llvm::outs() << "\nGenerating completed.";
            return EXIT_SUCCESS;
        }

        // JIT
        if(isRun)
        {
            if(!compiler.jit(unit)) return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }

        // Compile
        std::vector<std::string> outputs;
        if(!compiler.compile(unit, outputs)) return EXIT_FAILURE;
        if(isCompile)
        {
            llvm::outs() << "Compile completed.";
            return EXIT_SUCCESS;
        }

        // Linker
        if(!linker.linking(outputs)) return EXIT_FAILURE;
        llvm::outs() << "Build completed.";

        return EXIT_SUCCESS;
    }

    llvm::outs() << "Invalid arguments or too few or too many.\n";
    cli.printHelp();

    return EXIT_FAILURE;
}