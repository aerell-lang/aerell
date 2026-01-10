// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_LEXER_H
#define AERELL_LEXER_H

#include <file/file.h>

#include "aerell/token/token.h"

typedef struct
{
    const file_t* file;
    const char* content;
    token_t token;
} lexer_t;

void lexer_set_file(lexer_t* lexer, const file_t* file);

token_t* lexer_get_token(lexer_t* lexer);

#endif