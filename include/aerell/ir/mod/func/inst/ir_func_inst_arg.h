// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t arg_idx;
    uint32_t val_idx;
} ir_func_inst_arg_t;