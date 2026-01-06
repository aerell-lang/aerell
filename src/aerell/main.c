// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/lexer.h"
#include "aerell/file/file.h"

int main(int argc, const char* argv[])
{
    if(argc == 1) return 0;

    const char* path = argv[1];

    file_t* file = file_load(path);
    if(file == NULL)
    {
        printf("Failed to load file %s\n", path);
        return 1;
    }

    token_t tokens[10] = {0};
    size_t size = sizeof(tokens) / sizeof(tokens[0]);

    lexer(file, tokens, size);

    debug_tokens(tokens);

    file_free(file);
    return 0;
}