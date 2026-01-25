// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "aerell/backend/c/c_gen.h"

typedef struct
{
    char* data;
    uint32_t cap;
    uint32_t qty;
} cmod_t;

void cmod_write(cmod_t* self, const char* val)
{
    assert(self != NULL && val != NULL && "self or val is null");

    auto len = strlen(val);
    if(self->qty + len >= self->cap)
    {
        auto ncap = self->cap == 0 ? len * 2 : self->cap + len * 2;
        auto ndata = (char*)realloc((void*)self->data, sizeof(char) * ncap);
        if(ndata == NULL) return;
        self->cap = ncap;
        self->data = ndata;
    }
    strncpy(self->data + self->qty, val, len);
    self->qty += len;
    self->data[self->qty] = '\0';
}

void cmod_free(cmod_t* self)
{
    assert(self != NULL && "self is null");

    free(self->data);
    self->data = NULL;
    self->cap = 0;
    self->qty = 0;
}

const char* c_gen_generate(const ir_mod_t* mod)
{
    assert(mod != NULL && "mod is null");

    cmod_t cmod = {0};

    for(uint32_t i = 0; i < mod->funcs.qty; i++)
    {
        if(mod->funcs.pub[i] && mod->funcs.inst_qty[i] == 0) cmod_write(&cmod, "extern ");
        switch(mod->funcs.ret[i])
        {
        case IR_TYPE_VOID: cmod_write(&cmod, "void "); break;
        case IR_TYPE_I32: cmod_write(&cmod, "int "); break;
        case IR_TYPE_STR: cmod_write(&cmod, "const char* "); break;
        }
        cmod_write(&cmod, mod->vals.data[mod->funcs.name[i]].str);
        cmod_write(&cmod, "(");
        for(uint32_t j = 0; j < mod->funcs.param_qty[i]; j++)
        {
            if(j > 0) cmod_write(&cmod, ", ");
            switch(mod->funcs.param[i][j])
            {
            case IR_TYPE_VOID: cmod_write(&cmod, "void"); break;
            case IR_TYPE_I32: cmod_write(&cmod, "int"); break;
            case IR_TYPE_STR: cmod_write(&cmod, "const char*"); break;
            }
        }
        if(mod->funcs.vrdic[i]) cmod_write(&cmod, ",...");
        cmod_write(&cmod, ")");
        if(mod->funcs.inst_qty[i] == 0)
        {
            cmod_write(&cmod, ";");
            continue;
        }
        cmod_write(&cmod, "{");
        cmod_t arg = {0};
        for(uint32_t j = 0; j < mod->funcs.inst_qty[i]; j++)
        {
            switch(mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                if(mod->funcs.inst_data[i][j].arg.arg_idx > 0) cmod_write(&arg, ",");
                switch(mod->vals.type[mod->funcs.inst_data[i][j].arg.val_idx])
                {
                case IR_TYPE_VOID: break;
                case IR_TYPE_I32:
                    char x[12];
                    snprintf(x, sizeof(x), "%i", mod->vals.data[mod->funcs.inst_data[i][j].arg.val_idx].i32);
                    cmod_write(&arg, x);
                    break;
                case IR_TYPE_STR:
                    cmod_write(&arg, "\"");
                    cmod_write(&arg, mod->vals.data[mod->funcs.inst_data[i][j].arg.val_idx].str);
                    cmod_write(&arg, "\"");
                    break;
                }
                break;
            case IR_FUNC_INST_TYPE_CALL:
                cmod_write(&cmod, mod->vals.data[mod->funcs.name[mod->funcs.inst_data[i][j].idx]].str);
                cmod_write(&cmod, "(");
                cmod_write(&cmod, arg.data);
                cmod_write(&cmod, ");");
                cmod_free(&arg);
                break;
            }
        }
        cmod_free(&arg);
        cmod_write(&cmod, "}");
        cmod_write(&cmod, "int main(){init();return 0;}");
    }

    return cmod.data;
}