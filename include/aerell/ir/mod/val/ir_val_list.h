// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "aerell/ir/ir_type.h"
#include "aerell/ir/mod/val/ir_val_data.h"
#include "aerell/ir/mod/val/ir_val.h"

typedef struct
{
    ir_type_t* type;
    ir_val_data_t* data;

    uint32_t cap;
    uint32_t qty;
} ir_val_list_t;

bool ir_val_list_add(ir_val_list_t* self, uint32_t* idx, const ir_val_t* val);

void ir_val_list_dump(const ir_val_list_t* self);

void ir_val_list_free(ir_val_list_t* self);