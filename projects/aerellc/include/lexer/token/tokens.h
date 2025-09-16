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
 * File: tokens.h
 * Description: List of token struct and function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
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