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

#include "lexer/token/token_type.h"
#include "lexer/lexer.h"

int lexer_add_token(Tokens* tokens, int type, const char* content)
{
    Token* token = token_create(type, content);
    if(!token) return 0;
    return tokens_add(tokens, token);
}

int lexer_add_token_with_char(Tokens* tokens, int type, const char content)
{
    Token* token = token_create_with_char(type, content);
    if(!token) return 0;
    return tokens_add(tokens, token);
}

Tokens* lexer(FILE* file)
{
    Tokens* tokens = tokens_create();

    int c = fgetc(file);
    while(true)
    {
        // Break if char is EOF
        if(c == EOF)
        {
            lexer_add_token(tokens, TOKEN_EOF, NULL);
            break;
        }

        // Whitespace (SKIP)
        if(isspace(c))
        {
            c = fgetc(file);
            continue;
        }

        // Comments (SKIP)
        if(c == '#')
        {
            while(true)
            {
                c = fgetc(file);
                if(c == '\n' || c == EOF) break;
            }
            continue;
        }

        // * ( ) , ; f
        if(c == '*' || c == '(' || c == ')' || c == ',' || c == ';')
        {
            lexer_add_token_with_char(
                tokens,
                c == '*'   ? TOKEN_ASTERISK
                : c == '(' ? TOKEN_OPEN_PARENTHESES
                : c == ')' ? TOKEN_CLOSE_PARENTHESES
                : c == ',' ? TOKEN_COMA
                           : TOKEN_SEMICOLON, // ;
                c);
            c = fgetc(file);
            continue;
        }

        if(c == 'f')
        {
            int c1 = fgetc(file);
            if(isspace(c1))
            {
                lexer_add_token_with_char(tokens, TOKEN_F, 'f');
                c = c1;
                continue;
            }
            fseek(file, -1, SEEK_CUR);
        }

        // VARIADIC ...
        if(c == '.')
        {
            int c1 = fgetc(file);
            int c2 = fgetc(file);
            if(c1 == '.' && c2 == '.')
            {
                lexer_add_token(tokens, TOKEN_VARIADIC, "...");
                c = fgetc(file);
                continue;
            }
            // Backward (back to first '.')
            fseek(file, -2, SEEK_CUR);
        }

        // i1, i8, i16, i32, i64
        if(c == 'i')
        {
            int c1 = fgetc(file);
            int c2 = fgetc(file);

            if(c1 == '6' && c2 == '4')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I64, "i64");
                c = fgetc(file);
                continue;
            }

            if(c1 == '3' && c2 == '2')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I32, "i32");
                c = fgetc(file);
                continue;
            }

            if(c1 == '1' && c2 == '6')
            {
                lexer_add_token(tokens, TOKEN_DATA_TYPE_I16, "i16");
                c = fgetc(file);
                continue;
            }

            if(c1 == '1' || c1 == '8')
            {
                lexer_add_token(
                    tokens, (c1 == '1') ? TOKEN_DATA_TYPE_I1 : TOKEN_DATA_TYPE_I8, (c1 == '1') ? "i1" : "i8");
                c = c2;
                continue;
            }

            // Backward (back to 'i')
            fseek(file, -2, SEEK_CUR);
        }

        // Number -?[0-9]*(.[0-9]*)?
        if(isdigit(c) || c == '-')
        {
            if(c == '-')
            {
                int c1 = fgetc(file);
                if(!isdigit(c1)) goto skip_not_number;
                // Backward (back to '-')
                fseek(file, -1, SEEK_CUR);
            }

            // Setup buffer for number
            size_t buffer_size = 2;
            char* buffer = malloc(buffer_size * sizeof(char));
            size_t buffer_index = 0;
            if(!buffer) continue;

            bool decimal = false;
            buffer[buffer_index++] = c;
            while(true)
            {
                // Get char
                c = fgetc(file);

                if(c == '.' && !decimal) decimal = true;
                // Break if not digit or EOF
                else if((c == '.' && decimal) || !isdigit(c) || c == EOF)
                    break;

                // Auto resize buffer if not enough
                if((buffer_index + 1) >= buffer_size)
                {
                    size_t new_buffer_size = (buffer_size == 0) ? 2 : buffer_size * 2;
                    char* buffer_temp = realloc(buffer, new_buffer_size);
                    if(!buffer_temp) continue;
                    buffer = buffer_temp;
                    buffer_size = new_buffer_size;
                }
                buffer[buffer_index++] = c;
            }

            // Set null terminator at the end of buffer
            buffer[buffer_index] = '\0';

            // Create token
            lexer_add_token(tokens, TOKEN_VALUE_NUM, buffer);

            // Free
            free(buffer);

            continue;
        skip_not_number:
            // Backward (back to '-')
            fseek(file, -1, SEEK_CUR);
        }

        // String ' [^ \n\r\f\t].* '
        if(c == '\'')
        {
            // Setup buffer for string
            size_t buffer_size = 2;
            char* buffer = malloc(buffer_size * sizeof(char));
            size_t buffer_index = 0;
            if(!buffer) continue;

            while(true)
            {
                // Get char
                c = fgetc(file);

                // Break if quote
                if(c == '\'')
                {
                    // Skip char
                    c = fgetc(file);
                    break;
                }

                // Break if newline or EOF
                if(c == '\n' || c == '\r' || c == '\f' || c == EOF) break;

                // Next char
                if(c == '\\')
                {
                    c = fgetc(file);
                    if(c == 'n') c = '\n';
                    else if(c == 'r')
                        c = '\r';
                    else if(c == 'f')
                        c = '\f';
                    else if(c == 't')
                        c = '\t';
                }

                // Auto resize buffer if not enough
                if((buffer_index + 1) >= buffer_size)
                {
                    size_t new_buffer_size = (buffer_size == 0) ? 2 : buffer_size * 2;
                    char* buffer_temp = realloc(buffer, new_buffer_size);
                    if(!buffer_temp) continue;
                    buffer = buffer_temp;
                    buffer_size = new_buffer_size;
                }
                buffer[buffer_index++] = c;
            }

            // Set null terminator at the end of buffer
            buffer[buffer_index] = '\0';

            // Create token
            lexer_add_token(tokens, TOKEN_VALUE_STR, buffer);

            // Free
            free(buffer);

            continue;
        }

        // ID [a-zA-Z_][a-zA-Z0-9_]*
        if(isalpha(c) || c == '_')
        {
            // Setup buffer for ID
            size_t buffer_size = 1;
            char* buffer = malloc(buffer_size * sizeof(char));
            size_t buffer_index = 0;
            if(!buffer) continue;

            // Get char to buffer for ID
            buffer[buffer_index++] = c;
            while(true)
            {
                // Get char
                c = fgetc(file);

                // Break if not [a-zA-Z0-9_]
                if(!isalpha(c) && !isdigit(c) && c != '_') break;

                // Auto resize buffer if not enough
                if((buffer_index + 1) >= buffer_size)
                {
                    size_t new_buffer_size = (buffer_size == 0) ? 2 : buffer_size * 2;
                    char* buffer_temp = realloc(buffer, new_buffer_size);
                    if(!buffer_temp) continue;
                    buffer = buffer_temp;
                    buffer_size = new_buffer_size;
                }

                buffer[buffer_index++] = c;
            }

            // Set null terminator at the end of buffer
            buffer[buffer_index] = '\0';

            // Create token
            lexer_add_token(tokens, TOKEN_ID, buffer);

            // Free
            free(buffer);

            continue;
        }

        // Unknown char
        lexer_add_token_with_char(tokens, TOKEN_UNKNOWN, c);

        // Read next char
        c = fgetc(file);
    }

    tokens_shrink(tokens);
    return tokens;
}