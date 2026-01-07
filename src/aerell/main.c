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

    lexer_t lexer = {0};
    lexer_set_file(&lexer, file);

    token_t token = {0};
    do
    {
        lexer_get_token(&lexer, &token);
        debug_token(&token);
    } while(token.type != TOKEN_TYPE_EOF);

    file_free(file);
    return 0;
}