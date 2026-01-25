// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/ir/mod/ir_mod.h"

typedef struct
{

} llvm_ir_gen_t;

void llvm_ir_gen_generate(llvm_ir_gen_t* self, const ir_mod_t* ir_mod);