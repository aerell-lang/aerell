// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

#include "aerell/llvm/llvm_ir.h"

void llvm_ir_generate(const ir_mod_t* ir_mod)
{
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithNameInContext("a", context);
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);

    for(uint32_t i = 0; i < ir_mod->funcs.qty; i++)
    {
        // params
        uint32_t param_count = ir_mod->funcs.param_qty[i];
        LLVMTypeRef param_types[(param_count > 0) ? param_count : 1];

        for(uint32_t j = 0; j < param_count; j++)
        {
            LLVMTypeRef param_type = NULL;
            switch(ir_mod->funcs.param[i][j])
            {
            case IR_TYPE_VOID: param_type = LLVMVoidTypeInContext(context); break;
            case IR_TYPE_I32: param_type = LLVMInt32TypeInContext(context); break;
            case IR_TYPE_STR: param_type = LLVMPointerTypeInContext(context, 0); break;
            }
            param_types[j] = param_type;
        }

        // ret
        LLVMTypeRef ret_type = NULL;
        switch(ir_mod->funcs.ret[i])
        {
        case IR_TYPE_VOID: ret_type = LLVMVoidTypeInContext(context); break;
        case IR_TYPE_I32: ret_type = LLVMInt32TypeInContext(context); break;
        case IR_TYPE_STR: ret_type = LLVMPointerTypeInContext(context, 0); break;
        }

        // func type
        LLVMTypeRef func_type = LLVMFunctionType(ret_type, param_types, param_count, ir_mod->funcs.vrdic[i]);

        // func
        LLVMValueRef func = LLVMAddFunction(module, ir_mod->vals.data[ir_mod->funcs.name[i]].str, func_type);

        // block
        uint32_t inst_qty = ir_mod->funcs.inst_qty[i];
        if(inst_qty == 0) continue;

        LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, func, "entry");
        LLVMPositionBuilderAtEnd(builder, entry);

        uint32_t args_cap = 2;
        uint32_t args_qty = 0;
        LLVMValueRef* args = malloc(sizeof(LLVMValueRef) * args_cap);
        if(args == NULL) continue;

        for(uint32_t j = 0; j < inst_qty; j++)
        {
            switch(ir_mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                if(args_qty >= args_cap)
                {
                    uint32_t nargs_cap = args_cap * 2;
                    LLVMValueRef* nargs = realloc(args, sizeof(LLVMValueRef) * nargs_cap);
                    if(nargs == NULL) continue;
                    args_cap = nargs_cap;
                    args = nargs;
                }

                switch(ir_mod->vals.type[ir_mod->funcs.inst_data[i][j].arg.val_idx])
                {
                case IR_TYPE_VOID: break;
                case IR_TYPE_I32:
                    int32_t val_i32 = ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].arg.val_idx].i32;
                    args[ir_mod->funcs.inst_data[i][j].arg.arg_idx] =
                        LLVMConstInt(LLVMInt32TypeInContext(context), val_i32, 0);
                    break;
                case IR_TYPE_STR:
                    const char* val_c_str = ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].arg.val_idx].str;
                    LLVMValueRef val_llvm_str = LLVMConstStringInContext2(context, val_c_str, strlen(val_c_str), 0);
                    LLVMTypeRef val_llvm_type = LLVMTypeOf(val_llvm_str);
                    LLVMValueRef g_var = LLVMAddGlobal(module, val_llvm_type, "");
                    LLVMSetInitializer(g_var, val_llvm_str);
                    LLVMSetGlobalConstant(g_var, 1);
                    args[ir_mod->funcs.inst_data[i][j].arg.arg_idx] = g_var;
                    break;
                }

                args_qty++;
                break;
            case IR_FUNC_INST_TYPE_CALL:
                LLVMValueRef func =
                    LLVMGetNamedFunction(module, ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].idx].str);
                LLVMTypeRef type = LLVMGlobalGetValueType(func);
                LLVMBuildCall2(builder, type, func, args, args_qty, "");

                args_qty = 0;
                break;
            }
        }

        args_cap = 0;
        args_qty = 0;
        free(args);
        args = NULL;

        LLVMBuildRetVoid(builder);
    }

    LLVMDumpModule(module);

    char* error = NULL;
    if(LLVMVerifyModule(module, LLVMReturnStatusAction, &error))
    {
        printf("%s\n", error);
        LLVMDisposeMessage(error);
    }

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
}