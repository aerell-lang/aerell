/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>

#include "aerell/lexer/token/tokens.h"

tokens_t* tokens_create()
{
    tokens_t* tokens = malloc(sizeof(tokens_t));
    if(!tokens) return NULL;
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->offset = 0;
    tokens->data = NULL;
    return tokens;
}

int tokens_add(tokens_t* tokens, token_t* token)
{
    if(!tokens || !token) return 0;
    if(tokens->length >= tokens->capacity)
    {
        size_t new_capacity = (tokens->capacity == 0) ? 2 : tokens->capacity * 2;
        token_t** new_data = realloc(tokens->data, new_capacity * sizeof(token_t*));
        if(!new_data) return 0;
        tokens->data = new_data;
        tokens->capacity = new_capacity;
    }
    tokens->data[tokens->length++] = token;
    return 1;
}

token_t* tokens_get_token(tokens_t* tokens)
{
    if(!tokens) return NULL;
    return tokens->data[(tokens->offset == tokens->length - 1) ? tokens->offset : tokens->offset++];
}

void tokens_print(tokens_t* tokens)
{
    if(!tokens) return;
    for(size_t i = 0; i < tokens->length; i++) token_print(tokens->data[i]);
}

int tokens_shrink(tokens_t* tokens)
{
    if(!tokens) return 0;
    if(tokens->length == 0) return 1;
    token_t** new_data = realloc(tokens->data, tokens->length * sizeof(token_t*));
    if(!new_data) return 0;
    tokens->data = new_data;
    tokens->capacity = tokens->length;
    return 1;
}

void tokens_free(tokens_t* tokens)
{
    if(!tokens) return;
    if(tokens->data)
    {
        for(size_t i = 0; i < tokens->length; i++) token_free(tokens->data[i]);
        free(tokens->data);
    }
    tokens->length = 0;
    tokens->capacity = 0;
    tokens->offset = 0;
    free(tokens);
}