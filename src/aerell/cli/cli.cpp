/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <aerell/support/ostream.h>

#include "aerell/cli/cli.h"

namespace aerell
{

void CLI::printVersion() { outs() << "Aerell version: 0.0.0"; }

void CLI::printHelp()
{
    outs() << "A command-line utility for Aerell development.\n";
    outs() << "\n";
    outs() << "Usage: aerell <command>\n";
    outs() << "\n";
    outs() << "Available commands:\n";
    outs() << " help                Print this usage information.\n";
    outs() << " version             Print the Aerell compiler version.\n";
    outs() << " run <file>          Execute the source file using JIT compilation.\n";
    outs() << " build <file>        Build the source file into a standalone executable.\n";
    outs() << "\n";
    outs() << "Available internal commands:\n";
    outs() << " lex <file>          Tokenize the source file and print the lexer output.\n";
    outs() << " parse <file>        Parse the source file and print the AST structure.\n";
    outs() << " analyze <file>      Perform semantic analysis on the source file.\n";
    outs() << " generate <file>     Generate LLVM IR from source and print the IR output.\n";
    outs() << " compile <file>      Compile the source file into an object file.\n";
}

} // namespace aerell
