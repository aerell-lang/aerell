// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_TOKEN_TOKEN_TYPE_H
#define AERELL_TOKEN_TOKEN_TYPE_H

#include <stddef.h>

typedef enum
{
    TOKEN_TYPE_EOF,
    TOKEN_TYPE_INTL,
    TOKEN_TYPE_ILLEGAL,
} token_type_t;

const char* token_type_to_string(token_type_t t);

#endif