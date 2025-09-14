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

#include "aerellc/lexer/token/token.h"

typedef struct Tokens
{
    size_t length;
    size_t capacity;
    size_t offset;
    Token** data;
} Tokens;

Tokens* tokens_create();

int tokens_add(Tokens* tokens, Token* token);

Token* tokens_get_token(Tokens* tokens);

void tokens_print(Tokens* tokens);

int tokens_shrink(Tokens* tokens);

void tokens_free(Tokens* tokens);

#endif