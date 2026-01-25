// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef enum : uint8_t
{
    IR_TYPE_VOID,
    IR_TYPE_I32,
    IR_TYPE_STR
} ir_type_t;

const char* ir_type_to_c_str(ir_type_t type);