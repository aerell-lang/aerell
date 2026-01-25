// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef union {
    int32_t i32;
    const char* str;
} ir_val_data_t;