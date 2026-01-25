// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aerell/ir/mod/func/ir_func_list.h"

bool ir_func_list_add(ir_func_list_t* self, uint32_t* idx, const ir_func_t* func)
{
    assert(!(func->vrdic != false && (func->param_qty == 0 || func->param == NULL)) && "vrdic func must have 1 args");

    // Duplication check
    for(uint32_t i = 0; i < self->qty; i++)
    {
        if(self->name[i] == func->name)
        {
            *idx = i;
            return true;
        }
    }

    // Quantity and capacity check
    if(self->qty >= self->cap)
    {
        uint32_t ncap = self->cap == 0 ? 2 : self->cap * 2;

        uint32_t* nname = (uint32_t*)realloc(self->name, sizeof(uint32_t) * ncap);
        if(!nname) return false;

        ir_type_t** nparam = (ir_type_t**)realloc(self->param, sizeof(ir_type_t*) * ncap);
        if(!nparam)
        {
            free(nname);
            return false;
        }

        uint32_t* nparam_qty = (uint32_t*)realloc(self->param_qty, sizeof(uint32_t) * ncap);
        if(!nparam_qty)
        {
            free(nname);
            free(nparam);
            return false;
        }

        bool* nvrdic = (bool*)realloc(self->vrdic, sizeof(bool) * ncap);
        if(!nvrdic)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            return false;
        }

        ir_type_t* nret = (ir_type_t*)realloc(self->ret, sizeof(ir_type_t) * ncap);
        if(!nret)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            free(nvrdic);
            return false;
        }

        bool* npub = (bool*)realloc(self->pub, sizeof(bool) * ncap);
        if(!npub)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            free(nvrdic);
            free(nret);
            return false;
        }

        ir_func_inst_type_t** ninst_type =
            (ir_func_inst_type_t**)realloc(self->inst_type, sizeof(ir_func_inst_type_t*) * ncap);
        if(!ninst_type)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            free(nvrdic);
            free(nret);
            free(npub);
            return false;
        }

        ir_func_inst_data_t** ninst_data =
            (ir_func_inst_data_t**)realloc(self->inst_data, sizeof(ir_func_inst_data_t*) * ncap);
        if(!ninst_data)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            free(nvrdic);
            free(nret);
            free(npub);
            free(ninst_type);
            return false;
        }

        uint32_t* ninst_qty = (uint32_t*)realloc(self->inst_qty, sizeof(uint32_t) * ncap);
        if(!ninst_qty)
        {
            free(nname);
            free(nparam);
            free(nparam_qty);
            free(nvrdic);
            free(nret);
            free(npub);
            free(ninst_type);
            free(ninst_data);
            return false;
        }

        self->cap = ncap;
        self->name = nname;
        self->param = nparam;
        self->param_qty = nparam_qty;
        self->vrdic = nvrdic;
        self->ret = nret;
        self->pub = npub;
        self->inst_type = ninst_type;
        self->inst_data = ninst_data;
        self->inst_qty = ninst_qty;
    }

    // Copy param
    const auto cparam_size = sizeof(ir_type_t) * func->param_qty;
    auto cparam = (ir_type_t*)malloc(cparam_size);
    if(cparam == NULL) return false;
    memcpy(cparam, func->param, cparam_size);

    // Copy inst type
    const auto cinst_type_size = sizeof(ir_func_inst_type_t) * func->inst_qty;
    auto cintsType = (ir_func_inst_type_t*)malloc(cinst_type_size);
    if(cintsType == NULL)
    {
        free(cparam);
        return false;
    }
    for(uint32_t i = 0; i < func->inst_qty; i++) cintsType[i] = func->inst[i].type;

    // Copy inst data
    const auto cinst_data_size = sizeof(ir_func_inst_data_t) * func->inst_qty;
    auto cintsData = (ir_func_inst_data_t*)malloc(cinst_data_size);
    if(cintsData == NULL)
    {
        free(cparam);
        free(cintsType);
        return false;
    }
    for(uint32_t i = 0; i < func->inst_qty; i++) cintsData[i] = func->inst[i].data;

    // Add process
    *idx = self->qty;
    self->name[self->qty] = func->name;
    self->param[self->qty] = cparam;
    self->param_qty[self->qty] = func->param_qty;
    self->vrdic[self->qty] = func->vrdic;
    self->ret[self->qty] = func->ret;
    self->pub[self->qty] = func->pub;
    self->inst_type[self->qty] = cintsType;
    self->inst_data[self->qty] = cintsData;
    self->inst_qty[self->qty] = func->inst_qty;
    self->qty++;

    return true;
}

void ir_func_list_dump(const ir_func_list_t* self)
{
    printf("\nsec funcs\n");
    for(uint32_t i = 0; i < self->qty; i++)
    {
        printf("\n %u: vals[%u] (", i, self->name[i]);
        for(uint32_t j = 0; j < self->param_qty[i]; j++)
        {
            if(j > 0) printf(", ");
            printf("%s", ir_type_to_c_str(self->param[i][j]));
        }
        if(self->vrdic[i]) printf(", ...");
        printf(") %s", ir_type_to_c_str(self->ret[i]));
        if(self->pub[i]) printf(self->inst_qty[i] > 0 ? " pub:" : " ext");
        printf(self->inst_qty[i] > 0 ? "\n sec insts\n" : "\n");

        for(uint32_t j = 0; j < self->inst_qty[i]; j++)
        {
            switch(self->inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                printf(
                    "  %u: arg[%u] = vals[%u]\n", j, self->inst_data[i][j].arg.arg_idx,
                    self->inst_data[i][j].arg.val_idx);
                break;
            case IR_FUNC_INST_TYPE_CALL: printf("  %u: call funcs[%u]\n", j, self->inst_data[i][j].idx); break;
            }
        }
    }
}

void ir_func_list_free(ir_func_list_t* self)
{
    for(uint32_t i = 0; i < self->qty; i++)
    {
        free(self->param[i]);
        free(self->inst_type[i]);
        free(self->inst_data[i]);
    }

    free(self->name);
    free(self->param);
    free(self->param_qty);
    free(self->vrdic);
    free(self->ret);
    free(self->pub);
    free(self->inst_type);
    free(self->inst_data);
    free(self->inst_qty);

    self->name = NULL;
    self->param = NULL;
    self->param_qty = NULL;
    self->vrdic = NULL;
    self->ret = NULL;
    self->pub = NULL;
    self->inst_type = NULL;
    self->inst_data = NULL;
    self->inst_qty = NULL;

    self->qty = 0;
    self->cap = 0;
}