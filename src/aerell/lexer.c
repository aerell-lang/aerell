// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>

#include "aerell/lexer.h"

void lexer_set_file(lexer_t* lexer, const file_t* file)
{
    assert(lexer != NULL);
    assert(file != NULL);

    if(lexer == NULL || file == NULL) return;

    lexer->content = (lexer->file = file)->content;
}

/*
How it works:
If c is '5', it becomes 53 - 48 = 5. Since 5 < 9, it returns true.
If c is a character lower than '0', the subtraction results
in a very large number (due to unsigned overflow), which will be greater than 9.
*/
inline static bool is_digit(char c) { return (unsigned char)(c - '0') <= 9; }

inline static bool is_ws(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

void lexer_get_token(lexer_t* lexer, token_t* token)
{
    assert(lexer != NULL);
    assert(token != NULL);

    if(lexer == NULL || token == NULL) return;

    while(*lexer->content != '\0')
    {
        // Skip whitespace and new line
        if(is_ws(*lexer->content))
        {
            lexer->content++;
            continue;
        }

        // Skip comment
        if(*lexer->content == '#')
        {
            while(*lexer->content != '\n' && *lexer->content != '\0') lexer->content++;
            continue;
        }

        // Integer literal
        if(is_digit(*lexer->content))
        {
            size_t offset = (size_t)(lexer->content - lexer->file->content);

            lexer->content++;

            while(is_digit(*lexer->content)) lexer->content++;

            token->type = TOKEN_TYPE_INTL;
            token->offset = offset;
            token->size = ((size_t)(lexer->content - lexer->file->content)) - offset;
            token->file = lexer->file;

            return;
        }

        // Illegal
        token->type = TOKEN_TYPE_ILLEGAL;
        token->offset = (size_t)(lexer->content - lexer->file->content);
        token->size = 1;
        token->file = lexer->file;

        lexer->content++;
        return;
    }

    // End of file
    token->type = TOKEN_TYPE_EOF;
    token->offset = (size_t)(lexer->content - lexer->file->content);
    token->size = 0;
    token->file = lexer->file;
}