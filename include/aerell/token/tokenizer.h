// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/str.h"
#include "aerell/token/token.h"

typedef struct
{
    const str_t* buffer;
    uintptr_t index;
} tokenizer_t;

tokenizer_t tokenizer_init(const str_t* buffer);

void tokenizer_dump(const tokenizer_t* self, const token_t* token);

token_t tokenizer_next(tokenizer_t* self);