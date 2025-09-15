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

int lexer_add_token(tokens_t* tokens, int type, const char* content)
{
    token_t* token = token_create(type, content);
    if(!token) return 0;
    return tokens_add(tokens, token);
}

int lexer_add_token_with_char(tokens_t* tokens, int type, const char content)
{
    token_t* token = token_create_with_char(type, content);
    if(!token) return 0;
    return tokens_add(tokens, token);
}

tokens_t* lexer(SourceFile* source_file)
{
    if(!source_file || !source_file->buffer || source_file->buffer_size == 0) return NULL;

    tokens_t* tokens = tokens_create();

    const unsigned char* buffer = (const unsigned char*)source_file->buffer;
    const unsigned char* buffer_begin = buffer;
    const unsigned char* buffer_end = buffer + source_file->buffer_size;

    while(buffer < buffer_end)
    {
        unsigned char c = *buffer++;

        // Whitespace (skip)
        if(isspace(c)) continue;

        // Comments (skip)
        if(c == '#')
        {
            while(buffer < buffer_end && *buffer != '\n') buffer++;
            continue;
        }

        // Single-char tokens: * ( ) , ;
        if(c == '*' || c == '(' || c == ')' || c == ',' || c == ';')
        {
            lexer_add_token_with_char(
                tokens,
                c == '*'   ? TOKEN_ASTERISK
                : c == '(' ? TOKEN_OPEN_PARENTHESES
                : c == ')' ? TOKEN_CLOSE_PARENTHESES
                : c == ',' ? TOKEN_COMA
                           : TOKEN_SEMICOLON,
                c);
            continue;
        }

        // 'f' token
        if(c == 'f')
        {
            if(buffer < buffer_end && isspace(*buffer))
            {
                lexer_add_token_with_char(tokens, TOKEN_F, 'f');
                continue;
            }
        }

        // Variadic "..."
        if(c == '.' && (buffer + 1) < buffer_end)
        {
            if(buffer[0] == '.' && buffer[1] == '.')
            {
                lexer_add_token(tokens, TOKEN_VARIADIC, "...");
                buffer += 2;
                continue;
            }
        }

        // Integer types i1, i8, i16, i32, i64
        if(c == 'i' && (buffer + 1) < buffer_end)
        {
            unsigned char c1 = buffer[0];
            unsigned char c2 = buffer[1];

            if(c1 == '6' && c2 == '4')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I64, "i64");
                buffer += 2;
                continue;
            }
            if(c1 == '3' && c2 == '2')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I32, "i32");
                buffer += 2;
                continue;
            }
            if(c1 == '1' && c2 == '6')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I16, "i16");
                buffer += 2;
                continue;
            }
            if(c1 == '1')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I1, "i1");
                buffer += 1;
                continue;
            }
            if(c1 == '8')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I8, "i8");
                buffer += 1;
                continue;
            }
        }

        // Number: -?[0-9]*(.[0-9]*)?
        if(isdigit(c) || c == '-')
        {
            const unsigned char* num_start = buffer - 1;
            if(c == '-' && (buffer >= buffer_end || !isdigit(*buffer))) continue; // not a number

            while(buffer < buffer_end && (isdigit(*buffer) || *buffer == '.')) buffer++;
            size_t num_len = buffer - num_start;

            char* num_str = malloc(num_len + 1);
            if(!num_str) continue;
            for(size_t i = 0; i < num_len; i++) num_str[i] = num_start[i];
            num_str[num_len] = '\0';

            lexer_add_token(tokens, TOKEN_VALUE_NUM, num_str);
            free(num_str);
            continue;
        }

        // String '...'
        if(c == '\'')
        {
            const unsigned char* str_start = buffer;
            size_t buffer_size = 2;
            char* str_buf = malloc(buffer_size);
            if(!str_buf) continue;
            size_t index = 0;

            while(buffer < buffer_end)
            {
                c = *buffer++;
                if(c == '\'') break;                           // end of string
                if(c == '\n' || c == '\r' || c == '\f') break; // invalid end
                if(c == '\\' && buffer < buffer_end)
                { // escape
                    unsigned char esc = *buffer++;
                    c = (esc == 'n') ? '\n' : (esc == 'r') ? '\r' : (esc == 'f') ? '\f' : (esc == 't') ? '\t' : esc;
                }
                if(index + 1 >= buffer_size)
                {
                    buffer_size *= 2;
                    char* tmp = realloc(str_buf, buffer_size);
                    if(!tmp) break;
                    str_buf = tmp;
                }
                str_buf[index++] = c;
            }
            str_buf[index] = '\0';
            lexer_add_token(tokens, TOKEN_VALUE_STR, str_buf);
            free(str_buf);
            continue;
        }

        // Identifier [a-zA-Z_][a-zA-Z0-9_]*
        if(isalpha(c) || c == '_')
        {
            const unsigned char* id_start = buffer - 1;
            while(buffer < buffer_end && (isalnum(*buffer) || *buffer == '_')) buffer++;
            size_t id_len = buffer - id_start;

            char* id_str = malloc(id_len + 1);
            if(!id_str) continue;
            for(size_t i = 0; i < id_len; i++) id_str[i] = id_start[i];
            id_str[id_len] = '\0';

            lexer_add_token(tokens, TOKEN_ID, id_str);
            free(id_str);
            continue;
        }

        // Unknown char
        lexer_add_token_with_char(tokens, TOKEN_UNKNOWN, c);
    }

    // EOF
    lexer_add_token_with_char(tokens, TOKEN_EOF, '\0');

    tokens_shrink(tokens);
    return tokens;
}
