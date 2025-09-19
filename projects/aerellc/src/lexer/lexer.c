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
#include "lexer/lexer.h"
#include "support/unicode/unicode.h"
#include "support/unicode/utf8.h"

typedef struct multi_char_token
{
    const char* lexeme;
    const size_t lexeme_length;
    const token_type_t type;
    const bool can_be_a_identifier;
} multi_char_token_t;

const multi_char_token_t multi_char_tokens[] = {
    // Keywords
    {"f", 1, TOKEN_TYPE_KW_F, true},
    {"void", 4, TOKEN_TYPE_KW_VOID, true},
    {"use", 3, TOKEN_TYPE_KW_USE, true},
    {"var", 3, TOKEN_TYPE_KW_VAR, true},
    // Data types
    {"i32", 3, TOKEN_TYPE_DT_I32, true},
    {"f32", 3, TOKEN_TYPE_DT_F32, true},
    {"str", 3, TOKEN_TYPE_DT_STR, true},
};

const size_t multi_char_tokens_count = sizeof(multi_char_tokens) / sizeof(multi_char_tokens[0]);

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
    case '+': type = TOKEN_TYPE_ADD; break;
    case '=': type = TOKEN_TYPE_ASSIGN; break;
    case '(': type = TOKEN_TYPE_LPAREN; break;
    case '{': type = TOKEN_TYPE_LBRACE; break;
    case ')': type = TOKEN_TYPE_RPAREN; break;
    case '}': type = TOKEN_TYPE_RBRACE; break;
    case ';': type = TOKEN_TYPE_SEMICOLON; break;
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
        token_type_t type = TOKEN_TYPE_INT;

        while(((*buf) + lexeme_length) < buf_end && isdigit(*((*buf) + lexeme_length))) lexeme_length++;

        if(((*buf) + lexeme_length) < buf_end && (*((*buf) + lexeme_length) == '.'))
        {
            lexeme_length++;
            if(((*buf) + lexeme_length) < buf_end && !isdigit(*((*buf) + lexeme_length)))
            {
                lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, (*buf), lexeme_length);
                (*buf) += lexeme_length;
                return true;
            }

            type = TOKEN_TYPE_FLOAT;

            while(((*buf) + lexeme_length) < buf_end && isdigit(*((*buf) + lexeme_length))) lexeme_length++;
        }

        lexer_add_token(tokens, type, (*buf), lexeme_length);
        (*buf) += lexeme_length;
        return true;
    }
    return false;
}

bool is_char(const unsigned char** buf, const unsigned char* buf_end, tokens_t* tokens)
{
    if(**buf == '\'')
    {
        const unsigned char* start = *buf;
        size_t lexeme_length = 1;
        (*buf) += lexeme_length;

        if((*buf) >= buf_end)
        {
            lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, start, lexeme_length);
            return true;
        }

        if(**buf == '\\')
        {
            (*buf)++;
            lexeme_length++;
        }

        size_t ba = utf8_uchar_len(*(*buf));
        if(ba == 0)
        {
            (*buf)++;
            lexeme_length++;
            lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, start, lexeme_length);
            return true;
        }

        lexeme_length += ba;
        (*buf) += ba;

        if(**buf != '\'')
        {
            lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, start, lexeme_length);
            return true;
        }

        (*buf)++;
        lexeme_length++;

        lexer_add_token(tokens, TOKEN_TYPE_CHAR, start, lexeme_length);
        return true;
    }
    return false;
}

bool is_string(const unsigned char** buf, const unsigned char* buf_end, tokens_t* tokens)
{
    if(**buf == '"')
    {
        const unsigned char* start = *buf;
        size_t lexeme_length = 1;
        (*buf) += lexeme_length;

        while((*buf) < buf_end)
        {
            size_t ba = utf8_uchar_len(*(*buf));
            if(ba == 0)
            {
                (*buf)++;
                lexeme_length++;
                continue;
            }

            if(*(*buf) == '\n') break;
            if(*(*buf) == '"')
            {
                (*buf) += ba;
                lexeme_length += ba;
                goto string_done;
            }

            if(*(*buf) == '\\')
            {
                // escape next char
                if(((*buf) + ba) < buf_end)
                {
                    size_t escape_byte_amount = utf8_uchar_len(*((*buf) + ba));
                    if(escape_byte_amount > 0 && *((*buf) + ba) == '"')
                    {
                        (*buf) += ba + escape_byte_amount;
                        lexeme_length += ba + escape_byte_amount;
                        continue;
                    }
                }
            }

            (*buf) += ba;
            lexeme_length += ba;
        }

        lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, start, lexeme_length);
        return true;

    string_done:
        lexer_add_token(tokens, TOKEN_TYPE_STRING, start, lexeme_length);
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

        lexer_add_token(tokens, TOKEN_TYPE_ID, start, lexeme_length);
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
        if(skip(&buf, buf_end)) continue;

        if(is_multi_char_token(&buf, buf_end, tokens)) continue;

        if(is_single_char_token(&buf, tokens)) continue;

        if(is_number(&buf, buf_end, tokens)) continue;

        if(is_char(&buf, buf_end, tokens)) continue;

        if(is_string(&buf, buf_end, tokens)) continue;

        if(is_identifier(&buf, tokens)) continue;

        lexer_add_token(tokens, TOKEN_TYPE_ILLEGAL, buf, 1);
        buf++;
    }

    tokens_shrink(tokens);
    return tokens;
}
