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
#include <aerell/compiler/ir/ir_mod.h>
#include "aerell/compiler/symbol/symbol_print.h"
#include <aerell/support/ostream.h>

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
            aerell::outs() << "\nLexing finished.";
            return EXIT_SUCCESS;
        }

        // Parsing
        aerell::AST::Groups groups;
        if(!compiler.parsing(vecs, groups)) return EXIT_FAILURE;
        if(isParse)
        {
            aerell::outs() << groups << "\nParsing completed.";
            return EXIT_SUCCESS;
        }

        // Analysis
        if(!compiler.analysis(groups)) return EXIT_FAILURE;
        if(isAnalyze)
        {
            aerell::print(compiler.getSymbolTable());
            aerell::outs() << "\nAnalysis completed.";
            return EXIT_SUCCESS;
        }

        // Aerell IR Gen
        aerell::IRMod::Vec vec;
        if(!compiler.generating(groups, vec)) return EXIT_FAILURE;
        if(isGenerate) aerell::outs() << vec << "Aerell IR Generating completed.\n";

        // LLVM IR Gen
        aerell::IRllvm::Unit unit;
        if(!compiler.generating(vec, unit)) return EXIT_FAILURE;
        if(isGenerate) aerell::outs() << unit << "\nLLVM IR Generating completed.\n";

        // LLVM IR Link
        if(!compiler.linking(unit)) return EXIT_FAILURE;
        if(isGenerate) aerell::outs() << unit << "\nLLVM IR Linking completed.\n";

        // LLVM IR Optimize
        compiler.optimize(unit);
        if(isGenerate)
        {
            aerell::outs() << unit << "\nLLVM IR Optimizing completed.";
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
            aerell::outs() << "Compile completed.";
            return EXIT_SUCCESS;
        }

        // Linker
        if(!linker.linking(outputs)) return EXIT_FAILURE;
        aerell::outs() << "Build completed.";

        return EXIT_SUCCESS;
    }

    aerell::outs() << "Invalid arguments or too few or too many.\n";
    cli.printHelp();

    return EXIT_FAILURE;
}