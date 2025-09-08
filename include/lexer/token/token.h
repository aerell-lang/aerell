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

Token* token_create(int type, const char* content);

Token* token_create_with_char(int type, const char content);

void print_token(Token* token);

void token_free(Token* token);

#endif