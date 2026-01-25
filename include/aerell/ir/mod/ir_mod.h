// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "aerell/ir/mod/val/ir_val_list.h"
#include "aerell/ir/mod/func/ir_func_list.h"

typedef struct
{
    ir_val_list_t vals;
    ir_func_list_t funcs;
} ir_mod_t;

void ir_mod_dump(const ir_mod_t* self);

void ir_mod_free(ir_mod_t* self);