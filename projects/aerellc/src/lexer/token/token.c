/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/token/token_type.h"
#include "lexer/token/token.h"

token_t* token_create(token_type_t type, const unsigned char* content, size_t content_len)
{
    token_t* token = malloc(sizeof(token_t));
    if(!token) return NULL;
    token->type = type;
    token->content = content;
    token->content_len = content_len;
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
    if(strncmp(token_type_string[token->type], (const char*)token->content, token->content_len) == 0)
        printf("[%s]\n", token_type_string[token->type]);
    else
        printf("[%s] %.*s\n", token_type_string[token->type], (int)token->content_len, token->content);
}

void token_free(token_t* token)
{
    if(!token) return;
    token->type = TOKEN_TYPE_ILLEGAL;
    token->content = NULL;
    token->content_len = 0;
    free(token);
}