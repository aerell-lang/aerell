// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_LEXER_H
#define AERELL_LEXER_H

#include "aerell/token/token.h"
#include "aerell/file/file.h"

typedef struct
{
    const file_t* file;
    const char* content;
} lexer_t;

void lexer_set_file(lexer_t* lexer, const file_t* file);

void lexer_get_token(lexer_t* lexer, token_t* token);

#endif