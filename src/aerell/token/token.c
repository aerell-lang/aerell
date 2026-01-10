// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/token/token.h"

void debug_token(const token_t* token)
{
    if(token == NULL) return;

    printf("type: %s, offset: %zu, size: %zu", token_type_to_string(token->type), token->offset, token->size);

    const char* content = file_get_content(token->file) + token->offset;

    if(token->type == TOKEN_TYPE_ILLEGAL) printf(", char_number: %d", *content);

    printf(", content: %.*s\n", (int)token->size, content);
}