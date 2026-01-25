// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/ir/mod/func/inst/ir_func_inst_type.h"
#include "aerell/ir/mod/func/inst/ir_func_inst_data.h"

typedef struct
{
    ir_func_inst_type_t type;
    ir_func_inst_data_t data;
} ir_func_inst_t;