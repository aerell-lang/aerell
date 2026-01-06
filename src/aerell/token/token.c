// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/token/token.h"

void debug_token(const token_t* token)
{
    if(token == NULL) return;
    printf("{type: %s, offset: %zu, size: %zu}\n", to_string(token->type), token->offset, token->size);
}

void debug_tokens(const token_t* tokens)
{
    if(tokens == NULL) return;

    while(tokens->type != TOKEN_TYPE_EOF)
    {
        debug_token(tokens);
        tokens++;
    }

    debug_token(tokens);
}