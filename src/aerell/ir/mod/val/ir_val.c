// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir/mod/val/ir_val.h"

ir_val_t ir_val_i32(int32_t v) { return (ir_val_t){.type = IR_TYPE_I32, .data = {.i32 = v}}; }

ir_val_t ir_val_str(const char* v) { return (ir_val_t){.type = IR_TYPE_STR, .data = {.str = v}}; }