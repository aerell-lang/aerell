// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "aerell/ir/ir_type.h"
#include "aerell/ir/mod/func/ir_func.h"
#include "aerell/ir/mod/func/inst/ir_func_inst_type.h"
#include "aerell/ir/mod/func/inst/ir_func_inst_data.h"

typedef struct
{
    uint32_t* name;
    ir_type_t** param;
    uint32_t* param_qty;
    bool* vrdic;
    ir_type_t* ret;
    bool* pub;
    ir_func_inst_type_t** inst_type;
    ir_func_inst_data_t** inst_data;
    uint32_t* inst_qty;

    uint32_t cap;
    uint32_t qty;
} ir_func_list_t;

bool ir_func_list_add(ir_func_list_t* self, uint32_t* idx, const ir_func_t* func);

void ir_func_list_dump(const ir_func_list_t* self);

void ir_func_list_free(ir_func_list_t* self);