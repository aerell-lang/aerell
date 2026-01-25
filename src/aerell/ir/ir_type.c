// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir/ir_type.h"

const char* ir_type_to_c_str(ir_type_t type)
{
    switch(type)
    {
    case IR_TYPE_VOID: return "void";
    case IR_TYPE_I32: return "i32";
    case IR_TYPE_STR: return "str";
    }
    return "UNKNOWN";
}