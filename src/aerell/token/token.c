// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/token/token.h"

void print_token(const token_t* token)
{
    if(token == NULL) return;
    printf("{type: %s, offset: %zu, size: %zu}\n", to_string(token->type), token->offset, token->size);
}

void print_tokens(const token_t* tokens)
{
    if(tokens == NULL) return;

    while(tokens->type != TOKEN_TYPE_EOF)
    {
        print_token(tokens);
        tokens++;
    }

    print_token(tokens);
}