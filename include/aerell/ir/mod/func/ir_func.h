// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "aerell/ir/ir_type.h"
#include "aerell/ir/mod/func/inst/ir_func_inst.h"

typedef struct
{
    uint32_t name;
    ir_type_t* param;
    uint32_t param_qty;
    bool vrdic;
    ir_type_t ret;
    bool pub;
    ir_func_inst_t* inst;
    uint32_t inst_qty;
} ir_func_t;