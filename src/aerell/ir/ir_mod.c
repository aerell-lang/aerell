// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <stddef.h>

#include "aerell/ir/mod/ir_mod.h"

void ir_mod_dump(const ir_mod_t* self)
{
    assert(self != NULL && "self is null");
    ir_val_list_dump(&self->vals);
    ir_func_list_dump(&self->funcs);
}

void ir_mod_free(ir_mod_t* self)
{
    assert(self != NULL && "self is null");
    ir_val_list_free(&self->vals);
    ir_func_list_free(&self->funcs);
}