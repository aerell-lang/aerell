// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/lexer.h"

/*
How it works:
If c is '5', it becomes 53 - 48 = 5. Since 5 < 9, it returns true.
If c is a character lower than '0', the subtraction results
in a very large number (due to unsigned overflow), which will be greater than 9.
*/
inline static bool is_digit(char c) { return (unsigned char)(c - '0') <= 9; }

inline static bool is_ws(char c) { return c == ' ' || c == '\n'; }

void lexer(const char* data, token_t* tokens, size_t size)
{
    if(tokens == NULL || size == 0) return;

    const char* begin = data;

    size_t token_index = 0;

    while(size > 1 && *data != '\0' && token_index < (size - 1))
    {
        // Skip whitespace and new line
        if(is_ws(*data))
        {
            data++;
            continue;
        }

        // Integer literal
        if(is_digit(*data))
        {
            size_t offset = (size_t)(data - begin);

            data++;

            while(is_digit(*data)) data++;

            tokens[token_index].type = TOKEN_TYPE_INTL;
            tokens[token_index].offset = offset;
            tokens[token_index].size = ((size_t)(data - begin)) - offset;

            token_index++;
            continue;
        }

        // Illegal
        tokens[token_index].type = TOKEN_TYPE_ILLEGAL;
        tokens[token_index].offset = (size_t)(data - begin);
        tokens[token_index].size = 1;

        token_index++;
        data++;
    }

    // EOF
    tokens[token_index].type = TOKEN_TYPE_EOF;
    tokens[token_index].size = 0;
    tokens[token_index].offset = (size_t)(data - begin);
}