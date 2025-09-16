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
 * File: token.c
 * Description: Token function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aerellc/lexer/token/token_type.h"
#include "aerellc/lexer/token/token.h"

token_t* token_create(token_type_t type, const unsigned char* lexeme, size_t lexeme_length)
{
    token_t* token = malloc(sizeof(token_t));
    if(!token) return NULL;
    token->type = type;
    token->lexeme = lexeme;
    token->lexeme_length = lexeme_length;
    return token;
}

int token_type_is(token_t* token, token_type_t type)
{
    if(!token) return 0;
    return token->type == type;
}

int token_types_is(token_t* token, size_t types_len, token_type_t types[])
{
    if(!token) return 0;
    for(size_t i = 0; i < types_len; i++)
        if(token->type == types[i]) return 1;
    return 0;
}

void token_print(token_t* token)
{
    if(!token) return;
    printf("[%i] %.*s\n", token->type, (int)token->lexeme_length, token->lexeme);
}

void token_free(token_t* token)
{
    if(!token) return;
    token->type = TOKEN_UNKNOWN;
    token->lexeme = NULL;
    token->lexeme_length = 0;
    free(token);
}