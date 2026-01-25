// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aerell/ir/mod/val/ir_val_list.h"

bool ir_val_list_add(ir_val_list_t* self, uint32_t* idx, const ir_val_t* val)
{
    assert(
        self != NULL && idx != NULL && val != NULL && val->type != IR_TYPE_VOID &&
        "self, idx or val is null or val.type is void");

    // Duplication check
    for(uint32_t i = 0; i < self->qty; i++)
    {
        ir_type_t type = self->type[i];
        if(type != val->type) continue;

        switch(type)
        {
        case IR_TYPE_VOID: return false;
        case IR_TYPE_I32:
            if(self->data[i].i32 == val->data.i32)
            {
                *idx = i;
                return true;
            }
            break;
        case IR_TYPE_STR:
            if(strcmp(self->data[i].str, val->data.str) == 0)
            {
                *idx = i;
                return true;
            }
            break;
        }
    }

    // Quantity and capacity check
    if(self->qty >= self->cap)
    {
        auto ncap = self->cap == 0 ? 2 : self->cap * 2;

        auto ntype = (ir_type_t*)realloc(self->type, sizeof(ir_type_t) * ncap);
        if(!ntype) return false;

        auto ndata = (ir_val_data_t*)realloc(self->data, sizeof(ir_val_data_t) * ncap);
        if(!ndata)
        {
            free(ntype);
            return false;
        }

        self->cap = ncap;
        self->type = ntype;
        self->data = ndata;
    }

    // Add process
    *idx = self->qty;
    self->type[self->qty] = val->type;

    switch(self->type[self->qty])
    {
    case IR_TYPE_VOID: return false;
    case IR_TYPE_I32: self->data[self->qty].i32 = val->data.i32; break;
    case IR_TYPE_STR: self->data[self->qty].str = strdup(val->data.str); break;
    }

    self->qty++;

    return true;
}

void ir_val_list_dump(const ir_val_list_t* self)
{
    printf("\nsec vals\n");
    for(uint32_t i = 0; i < self->qty; i++)
    {
        auto type = self->type[i];
        printf(" %u: %s ", i, ir_type_to_c_str(type));
        switch(type)
        {
        case IR_TYPE_VOID: break;
        case IR_TYPE_I32: printf("%i", self->data[i].i32); break;
        case IR_TYPE_STR: printf("%s", self->data[i].str); break;
        }
        printf("\n");
    }
}

void ir_val_list_free(ir_val_list_t* self)
{
    for(uint32_t i = 0; i < self->qty; i++)
        if(self->type[i] == IR_TYPE_STR) free((char*)self->data[i].str);

    free(self->type);
    free(self->data);

    self->qty = 0;
    self->cap = 0;
}