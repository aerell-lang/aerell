// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <string.h>

#define INCLUDE_FILE
#include "aerell/file.c"

#define INCLUDE_TOKENIZER
#include "aerell/tokenizer.c"

#define INCLUDE_PARSE
#include "aerell/parse.c"

static const char* version = "Aerell version: 0.0.0";
static const char* help = "A command-line utility for Aerell development.\n\
\n\
Usage: aerell <command>\n\
\n\
Available commands:\n\
 h, help             Print this usage information.\n\
 v, version          Print the Aerell compiler version.\n\
 lex <file>          Tokenize the source file and print the lexer output.\n";

static inline void printVersion() { printf("%s", version); }

static inline void printHelp() { printf("%s", help); }

typedef enum
{
    COMMAND_NONE,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_LEX
} Command;

static inline int invalid()
{
    printf("Invalid arguments or too few or too many.\n");
    printHelp();
    return 0;
}

int main(int argc, const char* argv[])
{
    argc--;
    argv++;

    if(argc == 0)
    {
        printHelp();
        return 0;
    }

    Command command = COMMAND_NONE;

    if(argc == 1)
    {
        if(strcmp(argv[0], "h") == 0 || strcmp(argv[0], "help") == 0) command = COMMAND_HELP;
        else if(strcmp(argv[0], "v") == 0 || strcmp(argv[0], "version") == 0)
            command = COMMAND_VERSION;
        else
            return invalid();
    }
    else if(argc == 2)
    {
        if(strcmp(argv[0], "lex") == 0) command = COMMAND_LEX;
        else
            return invalid();
    }
    else if(argc > 2)
        return invalid();

    if(command == COMMAND_NONE || command == COMMAND_HELP)
    {
        printHelp();
        return 0;
    }

    if(command == COMMAND_VERSION)
    {
        printVersion();
        return 0;
    }

    const char* path = argv[1];

    File file = {0};
    if(!file_load(&file, path))
    {
        printf("Failed to open file.");
        return 1;
    }

    Tokenizer tokenizer = tokenizer_init(file.buffer, file.len);
    Token token = tokenizer_next(&tokenizer);
    while(token.tag != TOKEN_TAG_EOFF)
    {
        tokenizer_dump(&tokenizer, &token);
        token = tokenizer_next(&tokenizer);
    }
    tokenizer_dump(&tokenizer, &token);

    Parse parse = {0};
    parse.source = file.buffer;
    parse.token_index = 0;

    parse_root(&parse);

    file_unload(&file);
    return 0;
}
