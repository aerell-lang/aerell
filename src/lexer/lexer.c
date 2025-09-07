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
 * Description: Lexer function implementation.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#include <ctype.h>
#include <stdlib.h>

#include "token/token_type.h"
#include "lexer/lexer.h"

Tokens lexer(FILE* file)
{
    printf("Lexer is running.\n");
    Tokens tokens;
    tokens_init(&tokens);

    int c = fgetc(file);
    while(true)
    {
        // Break if char is EOF
        if(c == EOF)
        {
            printf("EOF...\n");
            // Create token
            Token token;
            token_init(&token);
            token_set_type(&token, TOKEN_TYPE_EOF);
            tokens_add(&tokens, token);
            break;
        }

        // Whitespace (SKIP)
        if(isspace(c))
        {
            printf("Skip whitespace...\n");
            c = fgetc(file);
            continue;
        }

        // Comments (SKIP)
        if(c == '#')
        {
            printf("Skip comment...\n");
            while(true)
            {
                c = fgetc(file);
                if(c == '\n' || c == EOF) break;
            }
            continue;
        }

        // ID [a-z]
        if(islower(c))
        {
            printf("Create ID token...\n");
            // Setup buffer for ID
            size_t buffer_size = 1;
            char* buffer = malloc(buffer_size * sizeof(char));
            size_t buffer_index = 0;
            if(!buffer)
            {
                printf("Failed to create buffer for ID.");
                continue;
            }

            // Get char to buffer for ID
            buffer[buffer_index++] = c;
            while(true)
            {
                // Get char
                c = fgetc(file);

                // Break if not lower alphabet [a-z]
                if(!islower(c)) break;

                // Auto resize buffer if not enough
                if((buffer_index + 1) >= buffer_size)
                {
                    size_t new_buffer_size = (buffer_size == 0) ? 2 : buffer_size * 2;
                    char* buffer_temp = realloc(buffer, new_buffer_size);
                    if(!buffer_temp)
                    {
                        printf("Failed to realloc buffer for ID.");
                        continue;
                    }
                    buffer = buffer_temp;
                    buffer_size = new_buffer_size;
                }
                buffer[buffer_index++] = c;
            }

            // Set null terminator at the end of buffer
            buffer[buffer_index] = '\0';

            // Create token
            Token token;
            token_init(&token);
            token_set_type(&token, TOKEN_TYPE_ID);
            token_set_content(&token, buffer);

            tokens_add(&tokens, token);

            // Free
            free(buffer);

            continue;
        }

        // String ' [^ \n\r\f\t].* '
        if(c == '\'')
        {
            printf("Create string token...\n");
            // Setup buffer for string
            size_t buffer_size = 2;
            char* buffer = malloc(buffer_size * sizeof(char));
            size_t buffer_index = 0;
            if(!buffer)
            {
                printf("Failed to create buffer for string.");
                continue;
            }

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

                // Auto resize buffer if not enough
                if((buffer_index + 1) >= buffer_size)
                {
                    size_t new_buffer_size = (buffer_size == 0) ? 2 : buffer_size * 2;
                    char* buffer_temp = realloc(buffer, new_buffer_size);
                    if(!buffer_temp)
                    {
                        printf("Failed to realloc buffer for string.");
                        continue;
                    }
                    buffer = buffer_temp;
                    buffer_size = new_buffer_size;
                }
                buffer[buffer_index++] = c;
            }

            // Set null terminator at the end of buffer
            buffer[buffer_index] = '\0';

            // Create token
            Token token;
            token_init(&token);
            token_set_type(&token, TOKEN_TYPE_STR);
            token_set_content(&token, buffer);

            tokens_add(&tokens, token);

            // Free
            free(buffer);

            continue;
        }

        // Unknown char
        printf("Create unknown token...\n");
        Token token;
        token_init(&token);
        token_set_type(&token, TOKEN_TYPE_UNKNOWN);
        token_set_content_with_char(&token, c);

        tokens_add(&tokens, token);

        // Read next char
        c = fgetc(file);
    }

    printf("Lexer stopped.\n");
    return tokens;
}