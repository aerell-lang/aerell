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
 * Description: List of token function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <stdio.h>
#include <stdlib.h>

#include "lexer/token/tokens.h"

Tokens* tokens_create()
{
    Tokens* tokens = malloc(sizeof(Tokens));
    if(tokens == NULL) return NULL;
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->offset = 0;
    tokens->data = NULL;
    return tokens;
}

int tokens_add(Tokens* tokens, Token* token)
{
    if(tokens == NULL || token == NULL) return 0;
    if(tokens->length >= tokens->capacity)
    {
        size_t new_capacity = (tokens->capacity == 0) ? 2 : tokens->capacity * 2;
        Token** data_temp = realloc(tokens->data, new_capacity * sizeof(Token*));
        if(!data_temp) return 0;
        tokens->data = data_temp;
        tokens->capacity = new_capacity;
    }
    tokens->data[tokens->length++] = token;
    return 1;
}

Token* tokens_get_token(Tokens* tokens)
{
    if(tokens == NULL) return NULL;
    return tokens->data[(tokens->offset == tokens->length - 1) ? tokens->offset : tokens->offset++];
}

void tokens_print(Tokens* tokens)
{
    if(tokens == NULL) return;
    for(size_t i = 0; i < tokens->length; i++) token_print(tokens->data[i]);
}

void tokens_free(Tokens* tokens)
{
    if(tokens == NULL) return;
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
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->offset = 0;
    free(tokens);
}

int tokens_shrink(Tokens* tokens)
{
    if(tokens == NULL) return 0;
    if(tokens->length == 0)
    {
        tokens_free(tokens);
        return 1;
    }
    Token** data_temp = realloc(tokens->data, tokens->length * sizeof(Token*));
    if(!data_temp) return 0;
    tokens->data = data_temp;
    tokens->capacity = tokens->length;
    return 1;
}