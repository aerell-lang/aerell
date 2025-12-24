/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>

#include "aerell/cli/cli.h"
#include "aerell/linker/linker.h"
#include "aerell/compiler/compiler.h"
#include "aerell/compiler/symbol/symbol_print.h"

namespace Aerell
{

void CLI::printVersion() { llvm::outs() << "Aerell version: 0.0.0"; }

void CLI::printHelp()
{
    llvm::outs() << "A command-line utility for Aerell development.\n";
    llvm::outs() << "\n";
    llvm::outs() << "Usage: aerell <command>\n";
    llvm::outs() << "\n";
    llvm::outs() << "Available commands:\n";
    llvm::outs() << " help                Print this usage information.\n";
    llvm::outs() << " version             Print the Aerell compiler version.\n";
    llvm::outs() << " run <file>          Execute the source file using JIT compilation.\n";
    llvm::outs() << " build <file>        Build the source file into a standalone executable.\n";
    llvm::outs() << "\n";
    llvm::outs() << "Available internal commands:\n";
    llvm::outs() << " lex <file>          Tokenize the source file and print the lexer output.\n";
    llvm::outs() << " parse <file>        Parse the source file and print the AST structure.\n";
    llvm::outs() << " analyze <file>      Perform semantic analysis on the source file.\n";
    llvm::outs() << " generate <file>     Generate LLVM IR from source and print the IR output.\n";
    llvm::outs() << " compile <file>      Compile the source file into an object file.\n";
}

} // namespace Aerell

int main(int argc, char* argv[])
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetDisassembler();

    Aerell::CLI cli;
    Aerell::Compiler compiler;
    Aerell::Linker linker;

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
        Aerell::Token::Vecs vecs;
        if(!compiler.lexing(filePath, vecs)) return EXIT_FAILURE;
        if(isLex)
        {
            Aerell::print(vecs);
            llvm::outs() << "\nLexing finished.";
            return EXIT_SUCCESS;
        }

        // Parsing
        Aerell::AST::Groups groups;
        if(!compiler.parsing(vecs, groups)) return EXIT_FAILURE;
        if(isParse)
        {
            Aerell::print(groups);
            llvm::outs() << "\nParsing completed.";
            return EXIT_SUCCESS;
        }

        // Analysis
        if(!compiler.analysis(groups)) return EXIT_FAILURE;
        if(isAnalyze)
        {
            Aerell::print(compiler.getSymbolTable());
            llvm::outs() << "\nAnalysis completed.";
            return EXIT_SUCCESS;
        }

        // IR Gen
        Aerell::IR::Unit unit;
        if(!compiler.generating(groups, unit)) return EXIT_FAILURE;
        if(!compiler.linking(unit)) return EXIT_FAILURE;
        compiler.optimize(unit);

        if(isGenerate)
        {
            Aerell::print(unit);
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