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

#include "lexer/token/token_type.h"
#include "lexer/token/token.h"

Token* token_create(int type, const char* content)
{
    Token* token = malloc(sizeof(Token));
    if(!token) return NULL;
    token->type = type;
    token->content = ((content) ? strdup(content) : NULL);
    if(content && !token->content)
    {
        token_free(token);
        token = NULL;
        return NULL;
    }
    return token;
}

Token* token_create_with_char(int type, const char content)
{
    Token* token = malloc(sizeof(Token));
    if(!token) return NULL;
    token->type = type;
    token->content = malloc(2);
    if(!token->content)
    {
        token_free(token);
        token = NULL;
        return NULL;
    }
    token->content[0] = content;
    token->content[1] = '\0';
    return token;
}

const char* token_get_content(const Token* token) {
    if(!token) return NULL;
    return token->content;
}

int token_is(Token* token, int type)
{
    if(!token) return 0;
    return token->type == type;
}

int token_iss(Token* token, size_t types_len, int types[])
{
    if(!token) return 0;
    for(size_t i = 0; i < types_len; i++)
        if(token->type == types[i]) return 1;
    return 0;
}

void token_print(Token* token)
{
    if(!token) return;
    printf(
        "[%i] %s\n", token->type,
        (token->content && strcmp(token->content, "\n") == 0) ? "\\n" : token->content);
}

void token_free(Token* token)
{
    if(!token) return;
    token->type = TOKEN_UNKNOWN;
    if(token->content)
    {
        free(token->content);
        token->content = NULL;
    }
    free(token);
}