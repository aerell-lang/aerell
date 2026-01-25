// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "aerell/c/c_emit.h"

typedef struct
{
    char* data;
    uint32_t cap;
    uint32_t qty;
} buff_t;

void buff_write(buff_t* self, const char* val)
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

void buff_free(buff_t* self)
{
    assert(self != NULL && "self is null");

    free(self->data);
    self->data = NULL;
    self->cap = 0;
    self->qty = 0;
}

const char* c_emit(const ir_mod_t* ir_mod)
{
    assert(ir_mod != NULL && "mod is null");

    buff_t root_buff = {0};

    for(uint32_t i = 0; i < ir_mod->funcs.qty; i++)
    {
        if(ir_mod->funcs.pub[i] && ir_mod->funcs.inst_qty[i] == 0) buff_write(&root_buff, "extern ");
        switch(ir_mod->funcs.ret[i])
        {
        case IR_TYPE_VOID: buff_write(&root_buff, "void "); break;
        case IR_TYPE_I32: buff_write(&root_buff, "int "); break;
        case IR_TYPE_STR: buff_write(&root_buff, "const char* "); break;
        }
        buff_write(&root_buff, ir_mod->vals.data[ir_mod->funcs.name[i]].str);
        buff_write(&root_buff, "(");
        for(uint32_t j = 0; j < ir_mod->funcs.param_qty[i]; j++)
        {
            if(j > 0) buff_write(&root_buff, ", ");
            switch(ir_mod->funcs.param[i][j])
            {
            case IR_TYPE_VOID: buff_write(&root_buff, "void"); break;
            case IR_TYPE_I32: buff_write(&root_buff, "int"); break;
            case IR_TYPE_STR: buff_write(&root_buff, "const char*"); break;
            }
        }
        if(ir_mod->funcs.vrdic[i]) buff_write(&root_buff, ",...");
        buff_write(&root_buff, ")");
        if(ir_mod->funcs.inst_qty[i] == 0)
        {
            buff_write(&root_buff, ";");
            continue;
        }
        buff_write(&root_buff, "{");
        buff_t arg_buff = {0};
        for(uint32_t j = 0; j < ir_mod->funcs.inst_qty[i]; j++)
        {
            switch(ir_mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                if(ir_mod->funcs.inst_data[i][j].arg.arg_idx > 0) buff_write(&arg_buff, ",");
                switch(ir_mod->vals.type[ir_mod->funcs.inst_data[i][j].arg.val_idx])
                {
                case IR_TYPE_VOID: break;
                case IR_TYPE_I32:
                    char x[12];
                    snprintf(x, sizeof(x), "%i", ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].arg.val_idx].i32);
                    buff_write(&arg_buff, x);
                    break;
                case IR_TYPE_STR:
                    buff_write(&arg_buff, "\"");
                    buff_write(&arg_buff, ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].arg.val_idx].str);
                    buff_write(&arg_buff, "\"");
                    break;
                }
                break;
            case IR_FUNC_INST_TYPE_CALL:
                buff_write(&root_buff, ir_mod->vals.data[ir_mod->funcs.name[ir_mod->funcs.inst_data[i][j].idx]].str);
                buff_write(&root_buff, "(");
                buff_write(&root_buff, arg_buff.data);
                buff_write(&root_buff, ");");
                buff_free(&arg_buff);
                break;
            }
        }
        buff_free(&arg_buff);
        buff_write(&root_buff, "}");
        buff_write(&root_buff, "int main(){init();return 0;}");
    }

    return root_buff.data;
}