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
 * File: token.h
 * Description: Token struct and function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#include "aerellc/lexer/token/token_type.h"

typedef struct token
{
    token_type_t type;
    const unsigned char* lexeme;
    size_t lexeme_length;
} token_t;

token_t* token_create(token_type_t type, const unsigned char* lexeme, size_t lexeme_length);

int token_type_is(token_t* token, token_type_t type);

int token_types_is(token_t* token, size_t types_len, token_type_t types[]);

void token_print(token_t* token);

void token_free(token_t* token);

#endif