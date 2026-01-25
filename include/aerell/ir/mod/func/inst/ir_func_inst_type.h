// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

typedef enum : uint8_t
{
    IR_FUNC_INST_TYPE_ARG,
    IR_FUNC_INST_TYPE_CALL,
} ir_func_inst_type_t;

const char* ir_func_inst_type_to_c_str(ir_func_inst_type_t type);