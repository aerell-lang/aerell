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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/token/token_type.h"
#include "support/range/ranges.h"
#include "support/unicode/unicode_ranges.h"
#include "lexer/lexer.h"

const bool ascii_whitespace[128] = {
    ['\t'] = true, ['\n'] = true, ['\v'] = true, ['\f'] = true, ['\r'] = true, [' '] = true};

const bool ascii_xid_start[128] = {['A' ... 'Z'] = true, ['a' ... 'z'] = true, ['_'] = true};

const bool ascii_xid_continue[128] = {['0' ... '9'] = true, ['A' ... 'Z'] = true, ['a' ... 'z'] = true, ['_'] = true};

size_t utf8_uchar_len(const unsigned char c)
{
    if((c & 0x80) == 0) return 1;    // 0xxxxxxx
    if((c & 0xE0) == 0xC0) return 2; // 110xxxxx
    if((c & 0xF0) == 0xE0) return 3; // 1110xxxx
    if((c & 0xF8) == 0xF0) return 4; // 11110xxx
    return 0;                        // invalid
}

bool is_unicode_whitespace(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_whitespace[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, whitespace_ranges, whitespace_ranges_count);
}

bool is_xid_start(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_xid_start[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, xid_start_ranges, xid_start_ranges_count);
}

bool is_xid_continue(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_xid_continue[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, xid_continue_ranges, xid_continue_ranges_count);
}

bool lexer_add_token(tokens_t* tokens, int type, const unsigned char* lexeme, size_t lexeme_length)
{
    token_t* token = token_create(type, lexeme, lexeme_length);
    if(!token) return false;
    return tokens_add(tokens, token);
}

tokens_t* lexer(source_file_t* source_file)
{
    if(!source_file || !source_file->buffer || source_file->buffer_size == 0) return NULL;

    tokens_t* tokens = tokens_create();

    const unsigned char* buffer = source_file->buffer;
    const unsigned char* buffer_end = buffer + source_file->buffer_size;

    while(buffer < buffer_end)
    {
        size_t byte_amount = 0; // byte amount
        size_t lexeme_length = 0;

        // WHITESPACE (Skip)
        if(is_unicode_whitespace(*buffer, &byte_amount))
        {
            buffer += byte_amount;
            continue;
        }

        // COMMENT (Skip)
        if(*buffer == '#')
        {
            while(buffer < buffer_end && *buffer != '\n') buffer++;
            continue;
        }

        // COMA, ASTERISK, SEMICOLON, PARENT_OPEN, PARENT_CLOSE
        {
            token_type_t type = TOKEN_ILLEGAL;

            switch(*buffer)
            {
            case '*': type = TOKEN_ASTERISK; break;
            case '(': type = TOKEN_PARENT_OPEN; break;
            case ')': type = TOKEN_PARENT_CLOSE; break;
            case ',': type = TOKEN_COMA; break;
            case ';': type = TOKEN_SEMICOLON; break;
            }

            if(type != TOKEN_ILLEGAL)
            {
                lexeme_length = 1;
                lexer_add_token(tokens, type, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
        }

        // KW_F
        if(*buffer == 'f' && (((buffer + 1) >= buffer_end) || is_unicode_whitespace(*(buffer + 1), &byte_amount)))
        {
            lexeme_length = 1;
            lexer_add_token(tokens, TOKEN_KW_F, buffer, lexeme_length);
            buffer += lexeme_length;
            continue;
        }

        // VARIADIC
        if(((buffer_end - buffer) >= 3) && (strncmp((const char*)buffer, "...", 3) == 0))
        {
            lexeme_length = 3;
            lexer_add_token(tokens, TOKEN_VARIADIC, buffer, lexeme_length);
            buffer += lexeme_length;
            continue;
        }

        // DT_I1, DT_I8, DT_I16, DT_I32, DT_I64
        if(((buffer_end - buffer) >= 2) && *buffer == 'i')
        {
            token_type_t type = TOKEN_ILLEGAL;

            if(*(buffer + 1) == '1')
            {
                if((buffer_end - buffer) >= 3 && *(buffer + 2) == '6')
                {
                    type = TOKEN_DATA_TYPE_I16;
                    lexeme_length = 3;
                }
                else
                {
                    type = TOKEN_DATA_TYPE_I1;
                    lexeme_length = 2;
                }
            }
            else if(*(buffer + 1) == '8')
            {
                type = TOKEN_DATA_TYPE_I8;
                lexeme_length = 2;
            }
            else if(*(buffer + 1) == '3' && (buffer_end - buffer) >= 3 && *(buffer + 2) == '2')
            {
                type = TOKEN_DATA_TYPE_I32;
                lexeme_length = 3;
            }
            else if(*(buffer + 1) == '6' && (buffer_end - buffer) >= 3 && *(buffer + 2) == '4')
            {
                type = TOKEN_DATA_TYPE_I64;
                lexeme_length = 3;
            }

            if(type != TOKEN_ILLEGAL && ((buffer + lexeme_length) < buffer_end) &&
               is_xid_continue(*(buffer + lexeme_length), &byte_amount))
                type = TOKEN_ILLEGAL;

            if(type != TOKEN_ILLEGAL)
            {
                lexer_add_token(tokens, type, buffer, lexeme_length);
                buffer += lexeme_length;
                continue;
            }
        }

        // NUMBER
        if(isdigit(*buffer))
        {
            lexeme_length = 1;

            while((buffer + lexeme_length) < buffer_end && isdigit(*(buffer + lexeme_length))) lexeme_length++;

            if((buffer + lexeme_length) < buffer_end && (*(buffer + lexeme_length) == '.'))
            {
                lexeme_length++;
                if((buffer + lexeme_length) < buffer_end && !isdigit(*(buffer + lexeme_length)))
                {
                    lexer_add_token(tokens, TOKEN_ILLEGAL, buffer, lexeme_length);
                    buffer += lexeme_length;
                    continue;
                }

                while((buffer + lexeme_length) < buffer_end && isdigit(*(buffer + lexeme_length))) lexeme_length++;
            }

            lexer_add_token(tokens, TOKEN_VALUE_NUM, buffer, lexeme_length);
            buffer += lexeme_length;
            continue;
        }

        // STRING
        if(*buffer == '\'')
        {
            const unsigned char* start = buffer;
            lexeme_length = 1;
            buffer += lexeme_length;

            while(buffer < buffer_end)
            {
                size_t byte_amount = utf8_uchar_len(*buffer);
                if(byte_amount == 0)
                {
                    buffer++;
                    lexeme_length++;
                    continue;
                }

                if(*buffer == '\n') break;
                if(*buffer == '\'')
                {
                    buffer += byte_amount;
                    lexeme_length += byte_amount;
                    goto string_done;
                }

                if(*buffer == '\\')
                {
                    // escape next char
                    if((buffer + byte_amount) < buffer_end)
                    {
                        size_t escape_byte_amount = utf8_uchar_len(*(buffer + byte_amount));
                        if(escape_byte_amount > 0 && *(buffer + byte_amount) == '\'')
                        {
                            buffer += byte_amount + escape_byte_amount;
                            lexeme_length += byte_amount + escape_byte_amount;
                            continue;
                        }
                    }
                }

                buffer += byte_amount;
                lexeme_length += byte_amount;
            }

            lexer_add_token(tokens, TOKEN_ILLEGAL, start, lexeme_length);
            continue;

        string_done:
            lexer_add_token(tokens, TOKEN_VALUE_STR, start, lexeme_length);
            continue;
        }

        // IDENTIFIER
        if(is_xid_start(*buffer, &byte_amount))
        {
            lexeme_length = byte_amount;

            const unsigned char* start = buffer;

            buffer += lexeme_length;

            while(is_xid_continue(*buffer, &byte_amount))
            {
                buffer += byte_amount;
                lexeme_length += byte_amount;
            }

            lexer_add_token(tokens, TOKEN_ID, start, lexeme_length);
            continue;
        }

        // Unknown char
        lexer_add_token(tokens, TOKEN_ILLEGAL, buffer, 1);
        buffer++;
    }

    // EOF
    lexer_add_token(tokens, TOKEN_EOF, buffer, 1);

    tokens_shrink(tokens);
    return tokens;
}
