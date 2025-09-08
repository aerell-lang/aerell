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

#include "lexer/token/tokens.h"
#include "lexer/lexer.h"
#include "config.h"

#include "parser/ast/ast.h"
#include "parser/ast/asts.h"
#include "parser/parser.h"

int main(int argc, char* argv[])
{
    // Exit if not argument
    if(argc <= 1)
    {
        printf("No arguments.\n");
        return -1;
    }

    // Exit if too many argument
    if(argc > 2)
    {
        printf("Too many argument.\n");
        return -1;
    }

    // Print version
    if((argc == 2) && (strcmp(argv[1], "version") == 0))
    {
        printf(__PROJECT_VERSION__ "\n");
        return 0;
    }

    // Print help
    if((argc == 2) && (strcmp(argv[1], "help") == 0))
    {
        printf(
            "Usage: " __PROJECT_NAME__ " [argument]\n\n"
            "Available arguments:\n"
            "  version   Print version.\n"
            "  help      Print help.\n"
            "  <file>    Run file.\n");
        return 0;
    }

    // Read file
    FILE* file = fopen(argv[1], "r");

    // Check file open or not
    if(file == NULL)
    {
        printf("Failed to opening file '%s'.\n", argv[1]);
        return -1;
    }

    // Tokenizer
    Tokens tokens = lexer(file);

    printf("\nPrint tokens:\n");
    printf("Tokens Length = %zu\n", tokens.length);
    printf("Tokens Capacity = %zu\n", tokens.capacity);

    print_tokens(&tokens);

    // Close file
    fclose(file);

    printf("\n");

    // Parsing to AST
    ASTs* asts = parser(&tokens);
    tokens_free(&tokens);

    printf("\nPrint asts:\n");
    printf("Asts Length = %zu\n", asts->length);
    printf("Asts Capacity = %zu\n", asts->capacity);
    print_asts(asts, 0);
    
    asts_free(asts);
    return 0;
}