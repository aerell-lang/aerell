// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/ir/mod/func/inst/ir_func_inst_arg.h"

typedef union {
    uint32_t idx;
    ir_func_inst_arg_t arg;
} ir_func_inst_data_t;