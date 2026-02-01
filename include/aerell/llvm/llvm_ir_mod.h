// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <llvm-c/Core.h>

typedef struct
{
    LLVMContextRef ctx;
    LLVMModuleRef mod;
} llvm_ir_mod_t;

bool llvm_ir_mod_init(llvm_ir_mod_t* self, const char* id);

void llvm_ir_mod_free(llvm_ir_mod_t* self);