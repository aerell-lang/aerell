// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <string.h>

#include "aerell/lexer.h"

void lexer_set_file(lexer_t* lexer, const file_t* file)
{
    assert(lexer != NULL);
    assert(file != NULL);

    if(lexer == NULL || file == NULL) return;

    lexer->content = file_get_content((lexer->file = file));
    memset(&lexer->token, 0, sizeof(lexer->token));
}

/*
How it works:
If c is '5', it becomes 53 - 48 = 5. Since 5 < 9, it returns true.
If c is a character lower than '0', the subtraction results
in a very large number (due to unsigned overflow), which will be greater than 9.
*/
inline static bool is_digit(char c) { return (unsigned char)(c - '0') <= 9; }

inline static bool is_ws(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

token_t* lexer_get_token(lexer_t* lexer)
{
    assert(lexer != NULL);

    if(lexer == NULL) return NULL;

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
            size_t offset = (size_t)(lexer->content - file_get_content(lexer->file));

            lexer->content++;

            while(is_digit(*lexer->content)) lexer->content++;

            lexer->token.type = TOKEN_TYPE_INTL;
            lexer->token.offset = offset;
            lexer->token.size = ((size_t)(lexer->content - file_get_content(lexer->file))) - offset;
            lexer->token.file = lexer->file;

            return &lexer->token;
        }

        // Illegal
        lexer->token.type = TOKEN_TYPE_ILLEGAL;
        lexer->token.offset = (size_t)(lexer->content - file_get_content(lexer->file));
        lexer->token.size = 1;
        lexer->token.file = lexer->file;

        lexer->content++;
        return &lexer->token;
    }

    // End of file
    lexer->token.type = TOKEN_TYPE_EOF;
    lexer->token.offset = (size_t)(lexer->content - file_get_content(lexer->file));
    lexer->token.size = 0;
    lexer->token.file = lexer->file;

    return &lexer->token;
}