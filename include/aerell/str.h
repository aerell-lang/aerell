// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef struct
{
    const char* data;
    uintptr_t len;
} str_t;

str_t str_from_c_str(const char* data);

str_t str_sub(const str_t* self, uintptr_t pos, uintptr_t count);

str_t str_sub_range(const str_t* self, uintptr_t start, uintptr_t end);