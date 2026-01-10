// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_TOKEN_TOKEN_H
#define AERELL_TOKEN_TOKEN_H

#include <stddef.h>

#include <file/file.h>

#include "aerell/token/token_type.h"

typedef struct
{
    token_type_t type;
    size_t offset;
    size_t size;
    const file_t* file;
} token_t;

void debug_token(const token_t* token);

#endif