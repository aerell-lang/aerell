// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <stddef.h>

#include "aerell/llvm/llvm_ir_mod.h"

bool llvm_ir_mod_init(llvm_ir_mod_t* self, const char* id)
{
    if(!self || !id) return false;

    LLVMContextRef ctx = LLVMContextCreate();
    if(!ctx) return false;

    LLVMModuleRef mod = LLVMModuleCreateWithNameInContext(id, ctx);
    if(!mod)
    {
        LLVMContextDispose(ctx);
        return false;
    }

    self->ctx = ctx;
    self->mod = mod;

    return true;
}

void llvm_ir_mod_free(llvm_ir_mod_t* self)
{
    if(!self) return;

    LLVMDisposeModule(self->mod);
    LLVMContextDispose(self->ctx);

    self->mod = NULL;
    self->ctx = NULL;
}