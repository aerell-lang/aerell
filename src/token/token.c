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

#include "token/token_type.h"
#include "token/token.h"

void token_init(struct Token* token)
{
    token->type = TOKEN_TYPE_UNKNOWN;
    token->content = NULL;
}

void token_set_type(struct Token* token, int type) { token->type = type; }

int token_set_content(struct Token* token, const char* content)
{
    token->content = strdup(content);
    if(!token->content)
    {
        printf("Failed to set content token, because strdup return null.\n");
        return 0;
    }
    return 1;
}

int token_set_content_with_char(struct Token* token, const char content)
{
    token->content = malloc(2 * sizeof(char));
    if(!token->content)
    {
        printf("Failed to set content token with char , because malloc return null.\n");
        return 0;
    }
    token->content[0] = content;
    token->content[1] = '\0';
    return 1;
}

void print_token(struct Token* token) { printf("[%i] %s\n", token->type, token->content); }

void token_free(struct Token* token)
{
    token->type = TOKEN_TYPE_UNKNOWN;
    if(token->content != NULL)
    {
        free(token->content);
        token->content = NULL;
    }
}