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
} c_mod_t;

void c_mod_write(c_mod_t* self, const char* val)
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

void c_mod_free(c_mod_t* self)
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

    c_mod_t c_mod = {0};

    for(uint32_t i = 0; i < mod->funcs.qty; i++)
    {
        if(mod->funcs.pub[i] && mod->funcs.inst_qty[i] == 0) c_mod_write(&c_mod, "extern ");
        switch(mod->funcs.ret[i])
        {
        case IR_TYPE_VOID: c_mod_write(&c_mod, "void "); break;
        case IR_TYPE_I32: c_mod_write(&c_mod, "int "); break;
        case IR_TYPE_STR: c_mod_write(&c_mod, "const char* "); break;
        }
        c_mod_write(&c_mod, mod->vals.data[mod->funcs.name[i]].str);
        c_mod_write(&c_mod, "(");
        for(uint32_t j = 0; j < mod->funcs.param_qty[i]; j++)
        {
            if(j > 0) c_mod_write(&c_mod, ", ");
            switch(mod->funcs.param[i][j])
            {
            case IR_TYPE_VOID: c_mod_write(&c_mod, "void"); break;
            case IR_TYPE_I32: c_mod_write(&c_mod, "int"); break;
            case IR_TYPE_STR: c_mod_write(&c_mod, "const char*"); break;
            }
        }
        if(mod->funcs.vrdic[i]) c_mod_write(&c_mod, ",...");
        c_mod_write(&c_mod, ")");
        if(mod->funcs.inst_qty[i] == 0)
        {
            c_mod_write(&c_mod, ";");
            continue;
        }
        c_mod_write(&c_mod, "{");
        c_mod_t arg = {0};
        for(uint32_t j = 0; j < mod->funcs.inst_qty[i]; j++)
        {
            switch(mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                if(mod->funcs.inst_data[i][j].arg.arg_idx > 0) c_mod_write(&arg, ",");
                switch(mod->vals.type[mod->funcs.inst_data[i][j].arg.val_idx])
                {
                case IR_TYPE_VOID: break;
                case IR_TYPE_I32:
                    char x[12];
                    snprintf(x, sizeof(x), "%i", mod->vals.data[mod->funcs.inst_data[i][j].arg.val_idx].i32);
                    c_mod_write(&arg, x);
                    break;
                case IR_TYPE_STR:
                    c_mod_write(&arg, "\"");
                    c_mod_write(&arg, mod->vals.data[mod->funcs.inst_data[i][j].arg.val_idx].str);
                    c_mod_write(&arg, "\"");
                    break;
                }
                break;
            case IR_FUNC_INST_TYPE_CALL:
                c_mod_write(&c_mod, mod->vals.data[mod->funcs.name[mod->funcs.inst_data[i][j].idx]].str);
                c_mod_write(&c_mod, "(");
                c_mod_write(&c_mod, arg.data);
                c_mod_write(&c_mod, ");");
                c_mod_free(&arg);
                break;
            }
        }
        c_mod_free(&arg);
        c_mod_write(&c_mod, "}");
        c_mod_write(&c_mod, "int main(){init();return 0;}");
    }

    return c_mod.data;
}