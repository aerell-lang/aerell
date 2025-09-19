/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h>

#include "lexer/token/token.h"

typedef struct tokens
{
    size_t length;
    size_t capacity;
    size_t offset;
    token_t** data;
} tokens_t;

tokens_t* tokens_create();

int tokens_add(tokens_t* tokens, token_t* token);

token_t* tokens_get_token(tokens_t* tokens);

void tokens_print(tokens_t* tokens);

int tokens_shrink(tokens_t* tokens);

void tokens_free(tokens_t* tokens);

#endif