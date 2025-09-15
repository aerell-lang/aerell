/*
 * Copyright (c) 2025, The Aerell Language Project Authors
 * All rights reserved.
 *
 * This file is part of the Aerell Language project.
 *
 * Licensed under the BSD 3-Clause License.
 * You may obtain a copy of the License at:
 * https://github.com/aerell-lang/aerell/blob/main/LICENSE
 *
 * File: main.c
 * Description: Entry point.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <string.h>

#include <stdio.h>
#include "aerellc/source/source_file.h"
#include <aerellc/lexer/lexer.h>
#include <aerellc/parser/parser.h>

int main(int argc, char* argv[])
{
    // Exit if not argument
    if(argc <= 1)
    {
        printf("No arguments.\n");
        return 1;
    }

    // Exit if too many argument
    if(argc > 2)
    {
        printf("Too many argument.\n");
        return 1;
    }

    // Print version
    if((argc == 2) && (strcmp(argv[1], "version") == 0))
    {
        printf("1.0.0\n");
        return 0;
    }

    // Print help
    if((argc == 2) && (strcmp(argv[1], "help") == 0))
    {
        printf(
            "Usage: aerell <command|file> [arguments]\n\n"
            "Global options:\n"
            "  -v, --verbose  Show verbose output.\n\n"
            "Available commands:\n"
            "  help      Print this usage information.\n"
            "  version   Print the Aerell version.\n");
        return 0;
    }

    source_file_t* source_file = source_file_create(argv[1]);
    if(!source_file)
    {
        printf("Failed to opening file '%s'.\n", argv[1]);
        return -1;
    }

    source_file_print(source_file);

    // Tokenizer
    tokens_t* tokens = lexer(source_file);

    source_file_free(source_file);

    // Print token
    printf("\n[Lexer] Tokenization Result:\n");
    tokens_print(tokens);

    printf("\n");

    // Parsing
    asts_t* asts = parser(tokens);

    // Print AST
    printf("[Parser] Parsing Result:\n");
    asts_print(asts, 0);

    // Clean up
    asts_free(asts);
    tokens_free(tokens);

    return 0;
}