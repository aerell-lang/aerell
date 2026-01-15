// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <print>

#include "aerell/cli.hpp"
#include "aerell/file.hpp"
#include "aerell/lexer.hpp"
#include "aerell/parser.hpp"
#include "aerell/semantic.hpp"
#include "aerell/ir.hpp"
#include "aerell/vm.hpp"

namespace aerell
{

void printVersion() { std::println("Aerell version: 0.0.0"); }

void printHelp()
{
    std::println("A command-line utility for Aerell development.");
    std::println("");
    std::println("Usage: aerell <command>");
    std::println("");
    std::println("Available commands:");
    std::println(" h, help             Print this usage information.");
    std::println(" v, version          Print the Aerell compiler version.");
    std::println(" lex <file>          Tokenize the source file and print the lexer output.");
    std::println(" parse <file>        Parse the source file and print the AST structure.");
    std::println(" analyze <file>      Perform semantic analysis on the source file.");
    std::println(" generate <file>     Generate IR from source and print the IR output.");
    std::println(" run <file>          Execute the source file using VM.");
}

enum class Command
{
    NONE,
    HELP,
    VERSION,
    LEX,
    PARSE,
    ANALYZE,
    GENERATE,
    RUN,
};

int invalid()
{
    std::println("Invalid arguments or too few or too many.");
    printHelp();
    return 0;
}

int CLI::main(int argc, const char* argv[])
{
    argc--;
    argv++;

    if(argc == 0)
    {
        printHelp();
        return 0;
    }

    Command command = Command::NONE;

    if(argc == 1)
    {
        if(std::strcmp(argv[0], "h") == 0 || std::strcmp(argv[0], "help") == 0) command = Command::HELP;
        else if(std::strcmp(argv[0], "v") == 0 || std::strcmp(argv[0], "version") == 0)
            command = Command::VERSION;
        else
            return invalid();
    }
    else if(argc == 2)
    {
        if(std::strcmp(argv[0], "lex") == 0) command = Command::LEX;
        else if(std::strcmp(argv[0], "parse") == 0)
            command = Command::PARSE;
        else if(std::strcmp(argv[0], "analyze") == 0)
            command = Command::ANALYZE;
        else if(std::strcmp(argv[0], "generate") == 0)
            command = Command::GENERATE;
        else if(std::strcmp(argv[0], "run") == 0)
            command = Command::RUN;
        else
            return invalid();
    }
    else if(argc > 2)
        return invalid();

    if(command == Command::NONE || command == Command::HELP)
    {
        printHelp();
        return 0;
    }

    if(command == Command::VERSION)
    {
        printVersion();
        return 0;
    }

    const char* path = argv[1];

    auto file = aerell::File::load(path);
    if(!file.has_value())
    {
        std::println("Failed to open file.");
        return 1;
    }

    aerell::Lexer lexer{file.value()};

    if(command == Command::LEX)
    {
        std::println("{}", file->getPath().generic_string());

        lexer.forwardToken();
        while(lexer.getToken().getKind() != TokenKind::EOFF)
        {
            std::println("{}", lexer.getToken().toStr());
            lexer.forwardToken();
        }

        std::print("{}\n\nLexing completed.", lexer.getToken().toStr());
        return 0;
    }

    aerell::Parser parser{lexer};
    auto ast = parser.parse();
    if(!ast.has_value()) return 1;

    if(command == Command::PARSE)
    {
        std::print("{}\nParsing completed.", ast.value().toStr());
        return 0;
    }

    aerell::Semantic semantic{ast.value()};
    if(!semantic.analyze()) return 1;

    if(command == Command::ANALYZE)
    {
        std::print("Analysis completed.");
        return 0;
    }

    aerell::IR ir{ast.value()};
    auto module = ir.gen();

    if(command == Command::GENERATE)
    {
        std::print("{}\nGenerating completed.", module.toStr());
        return 0;
    }

    aerell::VM vm{module};

    if(command == Command::RUN) return vm.run();

    return 1;
}

} // namespace aerell