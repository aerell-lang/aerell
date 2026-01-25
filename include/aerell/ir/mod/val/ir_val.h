// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/ir/ir_type.h"
#include "aerell/ir/mod/val/ir_val_data.h"

typedef struct
{
    ir_type_t type;
    ir_val_data_t data;
} ir_val_t;

ir_val_t ir_val_i32(int32_t v);

ir_val_t ir_val_str(const char* v);
