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

#include <stdio.h>
#include <string.h>

#include <source/source.h>
// #include <lexer/lexer.h>
// #include <parser/parser.h>

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
            "Usage: aerell [argument]\n\n"
            "Available arguments:\n"
            "  version   Print version.\n"
            "  help      Print help.\n"
            "  <file>    Run file.\n");
        return 0;
    }

    SourceFile* source_file = source_file_create(argv[1]);
    if(!source_file)
    {
        printf("Failed to opening file '%s'.\n", argv[1]);
        return -1;
    }

    source_file_print(source_file);

    source_file_free(source_file);

    // // Read file
    // FILE* file = fopen(argv[1], "r");

    // // Check file open or not
    // if(!file)
    // {
    //     printf("Failed to opening file '%s'.\n", argv[1]);
    //     return -1;
    // }

    // // Tokenizer
    // Tokens* tokens = lexer(file);

    // // Close file
    // fclose(file);

    // // Print token
    // printf("[Lexer] Tokenization Result:\n");
    // tokens_print(tokens);

    // printf("\n");

    // // Parsing
    // ASTs* asts = parser(tokens);

    // // Print AST
    // printf("[Parser] Parsing Result:\n");
    // asts_print(asts, 0);

    // // Clean up
    // asts_free(asts);
    // tokens_free(tokens);

    return 0;
}