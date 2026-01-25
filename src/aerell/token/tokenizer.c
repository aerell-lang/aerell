// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <stdio.h>

#include "aerell/token/tokenizer.h"

tokenizer_t tokenizer_init(const str_t* buffer)
{
    assert(buffer != nullptr && "buffer is null");
    return (tokenizer_t){.buffer = buffer, .index = 0};
}

void tokenizer_dump(const tokenizer_t* self, const token_t* token)
{
    assert(self != NULL && token != NULL && "self or token is null");

    str_t lexeme = str_sub_range(self->buffer, token->loc.start, token->loc.end);
    printf(
        "%s { start: %zu, end: %zu } %.*s\n", token_tag_to_c_str(token->tag), token->loc.start, token->loc.end,
        (int)lexeme.len, lexeme.data);
}

token_t tokenizer_next(tokenizer_t* self)
{
    token_t result = {.tag = TOKEN_TAG_INVALID, .loc = {.start = self->index, .end = 0}};

start:
    switch(self->buffer->data[self->index])
    {
    case 0:
        if(self->index == self->buffer->len)
        {
            result.tag = TOKEN_TAG_EOF;
            result.loc.start = self->index;
            goto ret;
        }
        goto invalid;
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        self->index += 1;
        result.loc.start = self->index;
        goto start;
    case '#': goto comment;
    case '0' ... '9':
        result.tag = TOKEN_TAG_NUML;
        self->index += 1;
        goto int_;
    }
    goto invalid;

expect_newline:
    self->index += 1;
    switch(self->buffer->data[self->index])
    {
    case 0:
        if(self->index == self->buffer->len)
        {
            result.tag = TOKEN_TAG_INVALID;
            goto ret;
        }
        goto invalid;
    case '\n':
        self->index += 1;
        result.loc.start = self->index;
        goto start;
    }
    goto invalid;

comment:
    self->index += 1;
    switch(self->buffer->data[self->index])
    {
    case 0:
        if(self->index != self->buffer->len) goto invalid;
        result.tag = TOKEN_TAG_EOF;
        result.loc.start = self->index;
        goto ret;
    case '\n':
        self->index += 1;
        result.loc.start = self->index;
        goto start;
    case '\r': goto expect_newline;
    }
    goto comment;

int_:
    switch(self->buffer->data[self->index])
    {
    case '0' ... '9': self->index += 1; goto int_;
    }
    goto ret;

invalid:
    self->index += 1;
    switch(self->buffer->data[self->index])
    {
    case 0:
        if(self->index == self->buffer->len)
        {
            result.tag = TOKEN_TAG_INVALID;
            goto ret;
        }
        goto invalid;
    case '\n': result.tag = TOKEN_TAG_INVALID; goto ret;
    }
    goto invalid;

ret:
    result.loc.end = self->index;
    return result;
}