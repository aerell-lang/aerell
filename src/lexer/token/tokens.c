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

#include "lexer/token/tokens.h"

void tokens_init(Tokens* tokens)
{
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->data = NULL;
}

int tokens_add(Tokens* tokens, Token* token)
{
    if(tokens->length >= tokens->capacity)
    {
        size_t new_capacity = (tokens->capacity == 0) ? 2 : tokens->capacity * 2;
        Token** data_temp = realloc(tokens->data, new_capacity * sizeof(Token*));
        if(!data_temp)
        {
            printf("Failed to realloc data to data_temp.\n");
            return 0;
        }
        tokens->data = data_temp;
        tokens->capacity = new_capacity;
    }
    tokens->data[tokens->length++] = token;
    return 1;
}

void print_tokens(Tokens* tokens)
{
    for(size_t i = 0; i < tokens->length; i++) print_token(tokens->data[i]);
}

void tokens_free(Tokens* tokens)
{
    tokens->length = 0;
    tokens->capacity = 0;
    for(size_t i = 0; i < tokens->length; i++)
    {
        if(tokens->data[i] != NULL)
        {
            token_free(tokens->data[i]);
            tokens->data[i] = NULL;
        }
    }
    if(tokens->data != NULL)
    {
        free(tokens->data);
        tokens->data = NULL;
    }
}

int tokens_shrink(Tokens* tokens)
{
    if(tokens->length == 0)
    {
        tokens_free(tokens);
        return 1;
    }

    Token** data_temp = realloc(tokens->data, tokens->length * sizeof(Token*));
    if(!data_temp)
    {
        printf("Failed to realloc data to data_temp.\n");
        return 0;
    }

    tokens->data = data_temp;
    tokens->capacity = tokens->length;

    return 1;
}