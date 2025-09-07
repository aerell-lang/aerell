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
 * File: tokens.c
 * Description: List of token function implementation.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <stdio.h>
#include <stdlib.h>

#include "token/tokens.h"

void tokens_init(struct Tokens* tokens)
{
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->data = NULL;
}

int tokens_add(struct Tokens* tokens, struct Token token)
{
    if(tokens->length >= tokens->capacity)
    {
        size_t new_capacity = (tokens->capacity == 0) ? 2 : tokens->capacity * 2;
        Token* data_temp = realloc(tokens->data, new_capacity * sizeof(Token));
        if(!data_temp)
        {
            printf("Failed to add token, because failed to realloc data.\n");
            return 0;
        }
        tokens->data = data_temp;
        tokens->capacity = new_capacity;
    }
    tokens->data[tokens->length++] = token;
    return 1;
}

void print_tokens(struct Tokens* tokens)
{
    for(size_t i = 0; i < tokens->length; i++) print_token(&tokens->data[i]);
}

void tokens_free(struct Tokens* tokens)
{
    tokens->length = 0;
    tokens->capacity = 0;
    for(size_t i = 0; i < tokens->length; i++) token_free(&tokens->data[i]);
    free(tokens->data);
    tokens->data = NULL;
}

int tokens_shrink(struct Tokens* tokens)
{
    if(tokens->length == 0)
    {
        tokens_free(tokens);
        return 1;
    }

    Token* data_temp = realloc(tokens->data, tokens->length * sizeof(Token));
    if(!data_temp)
    {
        printf("Failed to realloc data, for shrink tokens.");
        return 0;
    }

    tokens->data = data_temp;
    tokens->capacity = tokens->length;

    return 1;
}