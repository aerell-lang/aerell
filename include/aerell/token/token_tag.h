// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef enum : uint8_t
{
    TOKEN_TAG_INVALID,
    TOKEN_TAG_NUML,
    TOKEN_TAG_EOF,
} token_tag_t;

const char* token_tag_to_c_str(token_tag_t tag);