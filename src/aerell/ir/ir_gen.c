// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdlib.h>
#include <string.h>

#include "aerell/ir/ir_gen.h"

bool ir_gen_generate(ir_gen_t* self, file_t* file, ir_mod_t* mod)
{
    self->file = file;
    self->mod = mod;

    self->tokenizer = tokenizer_init(&self->file->content);

    // libc exit
    uint32_t exit_str_idx;
    ir_val_t exit_str_val = ir_val_str("exit");
    if(!ir_val_list_add(&self->mod->vals, &exit_str_idx, &exit_str_val)) return false;

    uint32_t exit_func_idx;
    ir_func_t exit_func = {.name = exit_str_idx, .param = (ir_type_t[1]){IR_TYPE_I32}, .param_qty = 1, .pub = true};
    if(!ir_func_list_add(&self->mod->funcs, &exit_func_idx, &exit_func)) return false;

    // libc printf
    uint32_t printf_str_idx;
    ir_val_t printf_str_val = ir_val_str("printf");
    if(!ir_val_list_add(&self->mod->vals, &printf_str_idx, &printf_str_val)) return false;

    uint32_t printf_func_idx;
    ir_func_t printf_func = {
        .name = printf_str_idx,
        .param = (ir_type_t[1]){IR_TYPE_STR},
        .param_qty = 1,
        .vrdic = true,
        .ret = IR_TYPE_I32,
        .pub = true};
    if(!ir_func_list_add(&self->mod->funcs, &printf_func_idx, &printf_func)) return false;

    // Entry point
    uint32_t inst_capacity = 6;
    uint32_t inst_qty = 0;
    ir_func_inst_t* inst = (ir_func_inst_t*)malloc(sizeof(ir_func_inst_t) * inst_capacity);
    if(inst == NULL) return false;

    while(true)
    {
        const auto token = tokenizer_next(&self->tokenizer);

        if(token.tag == TOKEN_TAG_NUML)
        {
            if(inst_qty + 3 >= inst_capacity)
            {
                auto ninst_capacity = inst_capacity == 0 ? 2 : inst_capacity * 2;
                auto ninst = (ir_func_inst_t*)realloc(inst, sizeof(ir_func_inst_t) * ninst_capacity);
                if(ninst == nullptr) continue;
                inst = ninst;
                inst_capacity = ninst_capacity;
            }

            str_t lexeme_str = str_sub_range(&self->file->content, token.loc.start, token.loc.end);

            char* lexeme_c_str = (char*)malloc(sizeof(char) * lexeme_str.len + 1);
            if(lexeme_c_str == nullptr) continue;
            strncpy(lexeme_c_str, lexeme_str.data, lexeme_str.len);
            lexeme_c_str[lexeme_str.len] = '\0';

            int32_t lexeme_int = atoi(lexeme_c_str);

            uint32_t val_int_idx;
            ir_val_t val_int = ir_val_i32(lexeme_int);
            if(!ir_val_list_add(&self->mod->vals, &val_int_idx, &val_int)) continue;

            uint32_t val_str_idx;
            ir_val_t val_str = ir_val_str("%i\\n");
            if(!ir_val_list_add(&self->mod->vals, &val_str_idx, &val_str)) continue;

            free(lexeme_c_str);

            inst[inst_qty++] = (ir_func_inst_t){
                .type = IR_FUNC_INST_TYPE_ARG,
                .data =
                    {.arg =
                         {
                             .arg_idx = 0,
                             .val_idx = val_str_idx,
                         }},
            };

            inst[inst_qty++] = (ir_func_inst_t){
                .type = IR_FUNC_INST_TYPE_ARG,
                .data =
                    {.arg =
                         {
                             .arg_idx = 1,
                             .val_idx = val_int_idx,
                         }},
            };

            inst[inst_qty++] = (ir_func_inst_t){
                .type = IR_FUNC_INST_TYPE_CALL,
                .data = {.idx = printf_func_idx},
            };
        }

        if(token.tag == TOKEN_TAG_EOF) break;
    }

    uint32_t val_int_idx;
    ir_val_t val_int = ir_val_i32(0);
    if(!ir_val_list_add(&self->mod->vals, &val_int_idx, &val_int))
    {
        free(inst);
        return false;
    }

    inst[inst_qty++] = (ir_func_inst_t){
        .type = IR_FUNC_INST_TYPE_ARG,
        .data =
            {.arg =
                 {
                     .arg_idx = 0,
                     .val_idx = val_int_idx,
                 }},
    };

    inst[inst_qty++] = (ir_func_inst_t){
        .type = IR_FUNC_INST_TYPE_CALL,
        .data = {.idx = exit_func_idx},
    };

    uint32_t val_str_idx;
    ir_val_t val_str = ir_val_str("init");
    if(!ir_val_list_add(&self->mod->vals, &val_str_idx, &val_str))
    {
        free(inst);
        return false;
    }

    uint32_t init_func_idx;
    ir_func_t init_func = {.name = val_str_idx, .pub = true, .inst = inst, .inst_qty = inst_qty};
    if(!ir_func_list_add(&self->mod->funcs, &init_func_idx, &init_func))
    {
        free(inst);
        return false;
    }

    free(inst);
    return true;
}