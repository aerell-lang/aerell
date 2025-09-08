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
 * Description: Token function implementation.
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
    if(!token)
    {
        printf("Failed to memory allocation for token.\n");
        return NULL;
    }
    token->type = type;
    token->content = ((content != NULL) ? strdup(content) : NULL);
    if(content != NULL && !token->content)
    {
        printf("Failed to strdup for token content.\n");
        token_free(token);
        token = NULL;
        return NULL;
    }
    return token;
}

Token* token_create_with_char(int type, const char content)
{
    Token* token = malloc(sizeof(Token));
    if(!token)
    {
        printf("Failed to memory allocation for token.\n");
        return NULL;
    }
    token->type = type;
    token->content = malloc(2);
    if(!token->content)
    {
        printf("Failed to memory allocation for token content.\n");
        token_free(token);
        token = NULL;
        return NULL;
    }
    token->content[0] = content;
    token->content[1] = '\0';
    return token;
}

void print_token(Token* token)
{
    printf(
        "[%i] %s\n", token->type,
        (token->content != NULL && strcmp(token->content, "\n") == 0) ? "\\n" : token->content);
}

void token_free(Token* token)
{
    if(token != NULL)
    {
        token->type = TOKEN_TYPE_UNKNOWN;
        if(token->content != NULL)
        {
            free(token->content);
            token->content = NULL;
        }
        free(token);
    }
}