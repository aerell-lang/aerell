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
 * Description: Token struct and function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token
{
    int type;
    char* content;
} Token;

void token_init(struct Token* token);

void token_set_type(struct Token* token, int type);

int token_set_content(struct Token* token, const char* content);

int token_set_content_with_char(struct Token* token, const char content);

void print_token(struct Token* token);

void token_free(struct Token* token);

#endif