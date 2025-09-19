/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
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

typedef struct multi_char_token
{
    const char* lexeme;
    const size_t lexeme_length;
    const token_type_t type;
    const bool can_be_a_identifier;
} multi_char_token_t;

const multi_char_token_t multi_char_tokens[] = {
    {"f", 1, TOKEN_KW_F, true},
    {"...", 3, TOKEN_VARIADIC, false},
    {"i1", 2, TOKEN_DATA_TYPE_I1, true},
    {"i8", 2, TOKEN_DATA_TYPE_I8, true},
    {"i16", 3, TOKEN_DATA_TYPE_I16, true},
    {"i32", 3, TOKEN_DATA_TYPE_I32, true},
    {"i64", 3, TOKEN_DATA_TYPE_I64, true},
};

const size_t multi_char_tokens_count = sizeof(multi_char_tokens) / sizeof(multi_char_tokens[0]);

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

bool skip_whitespace(const unsigned char** buf)
{
    size_t ba = 0;
    if(is_unicode_whitespace(**buf, &ba))
    {
        (*buf) += ba;
        return true;
    }
    return false;
}

bool skip_comment(const unsigned char** buf, const unsigned char* buf_end)
{
    if(**buf == '#')
    {
        while(*buf < buf_end && **buf != '\n') (*buf)++;
        return true;
    }
    return false;
}

bool skip(const unsigned char** buf, const unsigned char* buf_end)
{
    return skip_whitespace(buf) || skip_comment(buf, buf_end);
}

bool is_single_char_token(const unsigned char** buf, tokens_t* tokens)
{
    token_type_t type;

    switch(**buf)
    {
    case '*': type = TOKEN_ASTERISK; break;
    case '(': type = TOKEN_PARENT_OPEN; break;
    case ')': type = TOKEN_PARENT_CLOSE; break;
    case ',': type = TOKEN_COMA; break;
    case ';': type = TOKEN_SEMICOLON; break;
    default: return false;
    }

    size_t lexeme_length = 1;
    lexer_add_token(tokens, type, *buf, lexeme_length);
    (*buf) += lexeme_length;
    return true;
}

bool is_multi_char_token(const unsigned char** buf, const unsigned char* buf_end, tokens_t* tokens)
{
    for(size_t i = 0; i < multi_char_tokens_count; i++)
    {
        if(((size_t)(*buf - buf_end) >= multi_char_tokens[i].lexeme_length) &&
           (strncmp((const char*)*buf, multi_char_tokens[i].lexeme, multi_char_tokens[i].lexeme_length) == 0))
        {
            size_t ba;
            if(multi_char_tokens[i].can_be_a_identifier && ((*buf + multi_char_tokens[i].lexeme_length) < buf_end) &&
               is_xid_continue(*(*buf + multi_char_tokens[i].lexeme_length), &ba))
                return false;

            lexer_add_token(tokens, multi_char_tokens[i].type, *buf, multi_char_tokens[i].lexeme_length);
            (*buf) += multi_char_tokens[i].lexeme_length;
            return true;
        }
    }
    return false;
}

bool is_number(const unsigned char** buf, const unsigned char* buf_end, tokens_t* tokens)
{
    if(isdigit(**buf))
    {
        size_t lexeme_length = 1;

        while(((*buf) + lexeme_length) < buf_end && isdigit(*((*buf) + lexeme_length))) lexeme_length++;

        if(((*buf) + lexeme_length) < buf_end && (*((*buf) + lexeme_length) == '.'))
        {
            lexeme_length++;
            if(((*buf) + lexeme_length) < buf_end && !isdigit(*((*buf) + lexeme_length)))
            {
                lexer_add_token(tokens, TOKEN_ILLEGAL, (*buf), lexeme_length);
                (*buf) += lexeme_length;
                return true;
            }

            while(((*buf) + lexeme_length) < buf_end && isdigit(*((*buf) + lexeme_length))) lexeme_length++;
        }

        lexer_add_token(tokens, TOKEN_VALUE_NUM, (*buf), lexeme_length);
        (*buf) += lexeme_length;
        return true;
    }
    return false;
}

bool is_string(const unsigned char** buf, const unsigned char* buf_end, tokens_t* tokens)
{
    if(**buf == '\'')
    {
        const unsigned char* start = *buf;
        size_t lexeme_length = 1;
        (*buf) += lexeme_length;

        while((*buf) < buf_end)
        {
            size_t byte_amount = utf8_uchar_len(*(*buf));
            if(byte_amount == 0)
            {
                (*buf)++;
                lexeme_length++;
                continue;
            }

            if(*(*buf) == '\n') break;
            if(*(*buf) == '\'')
            {
                (*buf) += byte_amount;
                lexeme_length += byte_amount;
                goto string_done;
            }

            if(*(*buf) == '\\')
            {
                // escape next char
                if(((*buf) + byte_amount) < buf_end)
                {
                    size_t escape_byte_amount = utf8_uchar_len(*((*buf) + byte_amount));
                    if(escape_byte_amount > 0 && *((*buf) + byte_amount) == '\'')
                    {
                        (*buf) += byte_amount + escape_byte_amount;
                        lexeme_length += byte_amount + escape_byte_amount;
                        continue;
                    }
                }
            }

            (*buf) += byte_amount;
            lexeme_length += byte_amount;
        }

        lexer_add_token(tokens, TOKEN_ILLEGAL, start, lexeme_length);
        return true;

    string_done:
        lexer_add_token(tokens, TOKEN_VALUE_STR, start, lexeme_length);
        return true;
    }

    return false;
}

bool is_identifier(const unsigned char** buf, tokens_t* tokens)
{
    size_t ba;
    if(is_xid_start(**buf, &ba))
    {
        size_t lexeme_length = ba;

        const unsigned char* start = (*buf);

        (*buf) += lexeme_length;

        while(is_xid_continue(*(*buf), &ba))
        {
            (*buf) += ba;
            lexeme_length += ba;
        }

        lexer_add_token(tokens, TOKEN_ID, start, lexeme_length);
        return true;
    }
    return false;
}

tokens_t* lexer(source_file_t* source_file)
{
    if(!source_file || !source_file->buffer || source_file->buffer_size == 0) return NULL;

    tokens_t* tokens = tokens_create();

    const unsigned char* buf = source_file->buffer;
    const unsigned char* buf_end = buf + source_file->buffer_size;

    while(buf < buf_end)
    {
        // WHITESPACE, COMMENT (Skip)
        if(skip(&buf, buf_end)) continue;

        // KW_F, VARIADIC, DT_I1, DT_I8, DT_I16, DT_I32, DT_I64
        if(is_multi_char_token(&buf, buf_end, tokens)) continue;

        // COMA, ASTERISK, SEMICOLON, PARENT_OPEN, PARENT_CLOSE
        if(is_single_char_token(&buf, tokens)) continue;

        // NUMBER
        if(is_number(&buf, buf_end, tokens)) continue;

        // STRING
        if(is_string(&buf, buf_end, tokens)) continue;

        // IDENTIFIER
        if(is_identifier(&buf, tokens)) continue;

        // Illegal token
        lexer_add_token(tokens, TOKEN_ILLEGAL, buf, 1);
        buf++;
    }

    // EOF
    lexer_add_token(tokens, TOKEN_EOF, buf, 1);

    tokens_shrink(tokens);
    return tokens;
}
