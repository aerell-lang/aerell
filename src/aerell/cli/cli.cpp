/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include <aerell/reader/char_reader.h>
#include <aerell/support/ostream.h>

#include "aerell/cli/cli.h"
#include "aerell/source/source_manager.h"
#include <aerell/lexer/lexer.h>

namespace aerell
{

int CLI::main(int argc, const char* argv[])
{
    if(argc == 1 || (argc == 2 && (std::strcmp(argv[1], "h") == 0 || std::strcmp(argv[1], "help") == 0)))
    {
        this->printHelp();
        return 0;
    }

    if(argc == 2 && (std::strcmp(argv[1], "v") == 0 || std::strcmp(argv[1], "version") == 0))
    {
        this->printVersion();
        return 0;
    }

    bool isLex = false;
    if(argc > 1) isLex = std::strcmp(argv[1], "lex") == 0;

    if(argc == 3 && isLex)
    {
        SourceManager sourceManager;
        CharReader charReader;
        Lexer lexer{sourceManager, charReader};

        Lexer::Results results;
        if(!lexer.lexing(argv[2], results)) return 1;
        if(isLex)
        {
            outs() << results << "\nLexing finished.";
            return 0;
        }
    }

    outs() << "Invalid arguments or too few or too many.\n";
    this->printHelp();

    return 1;
}

void CLI::printVersion() { outs() << "Aerell version: 0.0.0"; }

void CLI::printHelp()
{
    outs() << "A command-line utility for Aerell development.\n";
    outs() << "\n";
    outs() << "Usage: aerell <command>\n";
    outs() << "\n";
    outs() << "Available commands:\n";
    outs() << " h, help             Print this usage information.\n";
    outs() << " v, version          Print the Aerell compiler version.\n";
    outs() << " lex <file>          Tokenize the source file and print the lexer output.\n";
}

} // namespace aerell
