// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/token/token.h"

void debug_token(const token_t* token)
{
    if(token == NULL) return;

    printf("{type: %s, offset: %zu, size: %zu} ", to_string(token->type), token->offset, token->size);

    const char* content = token->file->content + token->offset;

    if(token->type == TOKEN_TYPE_ILLEGAL) printf("(%d) ", *content);

    printf("%.*s\n", (int)token->size, content);
}

void debug_tokens(const token_t* tokens)
{
    if(tokens == NULL) return;

    printf("%s\n", tokens->file->path);

    while(tokens->type != TOKEN_TYPE_EOF)
    {
        debug_token(tokens);
        tokens++;
    }

    debug_token(tokens);
}