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
 * File: lexer.c
 * Description: Lexer function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "aerellc/lexer/token/token_type.h"
#include "aerellc/lexer/lexer.h"

int lexer_add_token(tokens_t* tokens, int type, const unsigned char* lexeme, size_t lexeme_length)
{
    token_t* token = token_create(type, lexeme, lexeme_length);
    if(!token) return 0;
    return tokens_add(tokens, token);
}

tokens_t* lexer(source_file_t* source_file)
{
    if(!source_file || !source_file->buffer || source_file->buffer_size == 0) return NULL;

    tokens_t* tokens = tokens_create();

    const unsigned char* buffer = (const unsigned char*)source_file->buffer;
    const unsigned char* buffer_end = buffer + source_file->buffer_size;

    while(buffer < buffer_end)
    {
        // Whitespace (skip)
        if(isspace(*buffer))
        {
            buffer++;
            continue;
        }

        // Comments (skip)
        if(*buffer == '#')
        {
            while(buffer < buffer_end && *buffer != '\n') buffer++;
            continue;
        }

        // Single-char tokens: * ( ) , ;
        if(*buffer == '*' || *buffer == '(' || *buffer == ')' || *buffer == ',' || *buffer == ';')
        {
            size_t lexeme_length = 1;
            lexer_add_token(
                tokens,
                *buffer == '*'   ? TOKEN_ASTERISK
                : *buffer == '(' ? TOKEN_OPEN_PARENTHESES
                : *buffer == ')' ? TOKEN_CLOSE_PARENTHESES
                : *buffer == ',' ? TOKEN_COMA
                                 : TOKEN_SEMICOLON,
                buffer, lexeme_length);
            buffer += lexeme_length;
            continue;
        }

        // 'f' token
        if(*buffer == 'f')
        {
            if((buffer + 1) < buffer_end && isspace(*(buffer + 1)))
            {
                size_t lexeme_length = 1;
                lexer_add_token(tokens, TOKEN_F, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
        }

        // Variadic "..."
        if(*buffer == '.' && (buffer + 1) < buffer_end && (buffer + 2) < buffer_end)
        {
            if(*(buffer + 1) == '.' && *(buffer + 2) == '.')
            {
                size_t lexeme_length = 3;
                lexer_add_token(tokens, TOKEN_VARIADIC, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
        }

        // Integer types i1, i8, i16, i32, i64
        if(*buffer == 'i' && (buffer + 1) < buffer_end)
        {
            size_t lexeme_length = 2;

            unsigned char c1 = *(buffer + 1);

            if(c1 == '1')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I1, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
            if(c1 == '8')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I8, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
            else
            {
                unsigned char c2 = *(buffer + 2);

                if((buffer + 2) < buffer_end)
                {
                    lexeme_length++;

                    if(c1 == '6' && c2 == '4')
                    {
                        lexer_add_token(tokens, TOKEN_DATA_TYPE_I64, buffer, lexeme_length);
                        buffer += lexeme_length;
                        continue;
                    }
                    if(c1 == '3' && c2 == '2')
                    {
                        lexer_add_token(tokens, TOKEN_DATA_TYPE_I32, buffer, lexeme_length);
                        buffer += lexeme_length;
                        continue;
                    }
                    if(c1 == '1' && c2 == '6')
                    {
                        lexer_add_token(tokens, TOKEN_DATA_TYPE_I16, buffer, lexeme_length);
                        buffer += lexeme_length;
                        continue;
                    }
                }
            }
        }

        // Number: -?[0-9]*(.[0-9]*)?
        if(isdigit(*buffer) || *buffer == '-')
        {
            size_t lexeme_length = 0;

            if(*buffer == '-')
            {
                if((buffer + 1) >= buffer_end || !isdigit(*(buffer + 1))) goto skip_is_not_number;
                lexeme_length++;
            }

            while((buffer + lexeme_length) < buffer_end && isdigit(*(buffer + lexeme_length))) lexeme_length++;

            if(*(buffer + lexeme_length) == '.')
            {
                lexeme_length++;

                while((buffer + lexeme_length) < buffer_end && isdigit(*(buffer + lexeme_length))) lexeme_length++;
            }

            lexer_add_token(tokens, TOKEN_VALUE_NUM, buffer, lexeme_length);
            buffer += lexeme_length;
            continue;

        skip_is_not_number:
        }

        // STRING
        if(*buffer == '\'')
        {
            size_t lexeme_length = 1;

            while((buffer + lexeme_length) < buffer_end && *(buffer + lexeme_length) != '\'' &&
                  *(buffer + lexeme_length) != '\n')
            {
                if(*(buffer + lexeme_length) == '\\' && (buffer + lexeme_length + 1) < buffer_end &&
                   *(buffer + lexeme_length + 1) == '\'')
                    lexeme_length++;
                lexeme_length++;
            }

            if(*(buffer + lexeme_length) == '\'')
            { // "'"
                lexeme_length++;
                lexer_add_token(tokens, TOKEN_VALUE_STR, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
        }

        // Identifier [a-zA-Z_][a-zA-Z0-9_]*
        if(isalpha(*buffer) || *buffer == '_')
        {
            size_t lexeme_length = 1;
            while((buffer + lexeme_length) < buffer_end &&
                  (isalnum(*(buffer + lexeme_length)) || *(buffer + lexeme_length) == '_'))
                lexeme_length++;

            lexer_add_token(tokens, TOKEN_ID, buffer, lexeme_length);
            buffer += lexeme_length;
            continue;
        }

        // Unknown char
        lexer_add_token(tokens, TOKEN_UNKNOWN, buffer, 1);
        buffer++;
    }

    // EOF
    lexer_add_token(tokens, TOKEN_EOF, buffer, 1);

    tokens_shrink(tokens);
    return tokens;
}
