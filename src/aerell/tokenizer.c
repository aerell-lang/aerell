// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifdef INCLUDE_TOKENIZER
#pragma once
#endif

#include <stddef.h>

#ifndef INCLUDE_TOKENIZER
#include <stdio.h>
#include <string.h>
#endif

typedef enum
{
    TOKEN_TAG_INVALID,
    TOKEN_TAG_EOFF,
    TOKEN_TAG_PLUS,
    TOKEN_TAG_NUML,
} TokenTag;

const char* TokenTagName(TokenTag tag)
#ifdef INCLUDE_TOKENIZER
    ;
#else
{
    switch(tag)
    {
    case TOKEN_TAG_INVALID: return "INVALID";
    case TOKEN_TAG_EOFF: return "EOFF";
    case TOKEN_TAG_PLUS: return "PLUS";
    case TOKEN_TAG_NUML: return "NUML";
    default: return "UNKNOWN";
    }
}
#endif

typedef struct
{
    size_t start;
    size_t end;
} TokenLoc;

typedef struct
{
    TokenTag tag;
    TokenLoc loc;
} Token;

typedef struct
{
    const char* buffer;
    size_t len;
    size_t index;
} Tokenizer;

typedef enum
{
    TOKENIZER_STATE_START,
    TOKENIZER_STATE_EXPECT_NEWLINE,
    TOKENIZER_STATE_COMMENT,
    TOKENIZER_STATE_INT,
    TOKENIZER_STATE_INVALID,
} TokenizerState;

Tokenizer tokenizer_init(const char* buffer, size_t len)
#ifdef INCLUDE_TOKENIZER
    ;
#else
{
    Tokenizer tokenizer;
    tokenizer.buffer = buffer;
    tokenizer.len = len;
    tokenizer.index = 0;
    return tokenizer;
}
#endif

void tokenizer_dump(const Tokenizer* self, const Token* token)
#ifdef INCLUDE_TOKENIZER
    ;
#else
{
    printf(
        "%s %.*s\n", TokenTagName(token->tag), (int)(token->loc.end - token->loc.start),
        self->buffer + token->loc.start);
}
#endif

Token tokenizer_next(Tokenizer* self)
#ifdef INCLUDE_TOKENIZER
    ;
#else
{
    Token result;
    result.tag = TOKEN_TAG_INVALID;
    result.loc.start = self->index;
    result.loc.end = 0;

    TokenizerState state = TOKENIZER_STATE_START;
state:
    switch(state)
    {
    case TOKENIZER_STATE_START:
        switch(self->buffer[self->index])
        {
        // Null terminator
        case '\0':
            if(self->index == self->len)
            {
                Token result;
                result.tag = TOKEN_TAG_EOFF;
                result.loc.start = self->index;
                result.loc.end = self->index;
                return result;
            }
            state = TOKENIZER_STATE_INVALID;
            goto state;
            break;
        // Whitespace
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            self->index += 1;
            result.loc.start = self->index;
            state = TOKENIZER_STATE_START;
            goto state;
            break;
        // Plus
        case '+':
            result.tag = TOKEN_TAG_PLUS;
            self->index += 1;
            break;
            // Hash
        case '#':
            state = TOKENIZER_STATE_COMMENT;
            goto state;
            break;
        // Digit
        case '0' ... '9':
            result.tag = TOKEN_TAG_NUML;
            self->index += 1;
            state = TOKENIZER_STATE_INT;
            goto state;
            break;
        default:
            state = TOKENIZER_STATE_INVALID;
            goto state;
            break;
        }
        break;
    case TOKENIZER_STATE_EXPECT_NEWLINE:
        self->index += 1;
        switch(self->buffer[self->index])
        {
            // Null terminator
        case '\0':
            if(self->index == self->len)
            {
                result.tag = TOKEN_TAG_INVALID;
            }
            else
            {
                state = TOKENIZER_STATE_INVALID;
                goto state;
            }
            break;
        // Newline
        case '\n':
            self->index += 1;
            result.loc.start = self->index;
            state = TOKENIZER_STATE_START;
            goto state;
            break;
        default:
            state = TOKENIZER_STATE_INVALID;
            goto state;
            break;
        }
        break;
    case TOKENIZER_STATE_COMMENT:
        self->index += 1;
        switch(self->buffer[self->index])
        {
            // Null terminator
        case '\0': break;
        // Carriage return
        case '\r':
            state = TOKENIZER_STATE_EXPECT_NEWLINE;
            goto state;
            break;
        // Newline
        case '\n':
            self->index += 1;
            result.loc.start = self->index;
            state = TOKENIZER_STATE_START;
            goto state;
            break;
        default:
            state = TOKENIZER_STATE_COMMENT;
            goto state;
            break;
        }
        break;
    case TOKENIZER_STATE_INT:
        switch(self->buffer[self->index])
        {
        // Digit
        case '0' ... '9':
            self->index += 1;
            state = TOKENIZER_STATE_INT;
            goto state;
            break;
        // Nothing
        default: break;
        }
        break;
    case TOKENIZER_STATE_INVALID:
        self->index += 1;
        switch(self->buffer[self->index])
        {
        // Null terminator
        case '\0':
            if(self->index == self->len)
            {
                result.tag = TOKEN_TAG_INVALID;
            }
            else
            {
                state = TOKENIZER_STATE_INVALID;
                goto state;
            }
            break;
        // Newline
        case '\n': result.tag = TOKEN_TAG_INVALID; break;
        default:
            state = TOKENIZER_STATE_INVALID;
            goto state;
            break;
        }
        break;
    }

    result.loc.end = self->index;
    return result;
}
#endif
