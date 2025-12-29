/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <llvm/Support/raw_ostream.h>

#include "aerell/cli/cli.h"

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
