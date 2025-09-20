/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#include "lexer/token/token_type.h"

typedef struct token
{
    token_type_t type;
    const unsigned char* content;
    size_t content_len;
} token_t;

token_t* token_create(token_type_t type, const unsigned char* content, size_t content_len);

int token_type_is(token_t* token, token_type_t type);

int token_types_is(token_t* token, size_t types_len, token_type_t types[]);

void token_print(token_t* token);

void token_free(token_t* token);

#endif