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

#include "token/tokens.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[])
{
    // Exit if not argument
    if(argc <= 1) return 0;

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
    tokens_shrink(&tokens);

    printf("Print tokens:\n");
    printf("Tokens Length = %zu\n", tokens.length);
    printf("Tokens Capacity = %zu\n", tokens.capacity);

    print_tokens(&tokens);

    tokens_free(&tokens);

    // Close file
    fclose(file);

    return 0;
}