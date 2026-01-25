// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir/mod/func/inst/ir_func_inst_type.h"

const char* ir_func_inst_type_to_c_str(ir_func_inst_type_t type)
{
    switch(type)
    {
    case IR_FUNC_INST_TYPE_ARG: return "arg";
    case IR_FUNC_INST_TYPE_CALL: return "call";
    }
    return "unknown";
}