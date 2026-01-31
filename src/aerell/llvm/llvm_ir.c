// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include "llvm-c/TargetMachine.h"
#include "llvm-c/Transforms/PassBuilder.h"

#include "aerell/llvm/llvm_ir.h"

bool llvm_ir_optimization(LLVMModuleRef llvm_mod)
{
    LLVMPassBuilderOptionsRef opts = LLVMCreatePassBuilderOptions();

    LLVMPassBuilderOptionsSetVerifyEach(opts, 1);

    char* triple = LLVMGetDefaultTargetTriple();
    if(triple == NULL) return false;

    LLVMTargetRef target = NULL;
    char* err_msg = NULL;
    if(LLVMGetTargetFromTriple(triple, &target, &err_msg) != 0)
    {
        printf("%s\n", err_msg);
        LLVMDisposeMessage(err_msg);
        LLVMDisposeMessage(triple);
        LLVMDisposePassBuilderOptions(opts);
        return false;
    }

    LLVMTargetMachineRef tm = LLVMCreateTargetMachine(
        target, triple, "generic", "", LLVMCodeGenLevelAggressive, LLVMRelocDefault, LLVMCodeModelSmall);

    LLVMErrorRef err = LLVMRunPasses(llvm_mod, "default<O3>", tm, opts);
    if(err != NULL)
    {
        err_msg = LLVMGetErrorMessage(err);
        printf("%s\n", err_msg);
        LLVMDisposeErrorMessage(err_msg);
        LLVMDisposeTargetMachine(tm);
        LLVMDisposeMessage(triple);
        LLVMDisposePassBuilderOptions(opts);
        return false;
    }

    LLVMDisposeTargetMachine(tm);
    LLVMDisposeMessage(triple);
    LLVMDisposePassBuilderOptions(opts);
    return true;
}

bool llvm_ir_generate(const ir_mod_t* ir_mod)
{
    LLVMContextRef ctx = LLVMContextCreate();
    if(ctx == NULL) return false;

    LLVMModuleRef mod = LLVMModuleCreateWithNameInContext("a", ctx);
    if(mod == NULL)
    {
        LLVMContextDispose(ctx);
        return false;
    }

    LLVMBuilderRef builder = LLVMCreateBuilderInContext(ctx);
    if(builder == NULL)
    {
        LLVMDisposeModule(mod);
        LLVMContextDispose(ctx);
        return false;
    }

    for(uint32_t i = 0; i < ir_mod->funcs.qty; i++)
    {
        // params
        uint32_t param_qty = ir_mod->funcs.param_qty[i];
        LLVMTypeRef param_types[(param_qty > 0) ? param_qty : 1];

        for(uint32_t j = 0; j < param_qty; j++)
        {
            LLVMTypeRef param_type = NULL;
            switch(ir_mod->funcs.param[i][j])
            {
            case IR_TYPE_VOID: param_type = LLVMVoidTypeInContext(ctx); break;
            case IR_TYPE_I32: param_type = LLVMInt32TypeInContext(ctx); break;
            case IR_TYPE_STR: param_type = LLVMPointerTypeInContext(ctx, 0); break;
            }
            param_types[j] = param_type;
        }

        // ret
        LLVMTypeRef ret_type = NULL;
        switch(ir_mod->funcs.ret[i])
        {
        case IR_TYPE_VOID: ret_type = LLVMVoidTypeInContext(ctx); break;
        case IR_TYPE_I32: ret_type = LLVMInt32TypeInContext(ctx); break;
        case IR_TYPE_STR: ret_type = LLVMPointerTypeInContext(ctx, 0); break;
        }

        // func type
        LLVMTypeRef func_type = LLVMFunctionType(ret_type, param_types, param_qty, ir_mod->funcs.vrdic[i]);

        // func
        LLVMValueRef func = LLVMAddFunction(mod, ir_mod->vals.data[ir_mod->funcs.name[i]].str, func_type);

        // block
        uint32_t inst_qty = ir_mod->funcs.inst_qty[i];
        if(inst_qty == 0) continue;

        LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(ctx, func, "entry");
        LLVMPositionBuilderAtEnd(builder, entry);

        uint32_t args_cap = 2;
        uint32_t args_qty = 0;
        LLVMValueRef* args = (LLVMValueRef*)malloc(sizeof(LLVMValueRef) * args_cap);
        if(args == NULL) continue;

        for(uint32_t j = 0; j < inst_qty; j++)
        {
            switch(ir_mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG:
                if(args_qty >= args_cap)
                {
                    uint32_t nargs_cap = args_cap * 2;
                    LLVMValueRef* nargs = (LLVMValueRef*)realloc(args, sizeof(LLVMValueRef) * nargs_cap);
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
                        LLVMConstInt(LLVMInt32TypeInContext(ctx), val_i32, 0);
                    break;
                case IR_TYPE_STR:
                    const char* val_c_str = ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].arg.val_idx].str;
                    LLVMValueRef val_llvm_str = LLVMConstStringInContext2(ctx, val_c_str, strlen(val_c_str), 0);
                    LLVMTypeRef val_llvm_type = LLVMTypeOf(val_llvm_str);
                    LLVMValueRef g_var = LLVMAddGlobal(mod, val_llvm_type, "");
                    LLVMSetInitializer(g_var, val_llvm_str);
                    LLVMSetGlobalConstant(g_var, 1);
                    args[ir_mod->funcs.inst_data[i][j].arg.arg_idx] = g_var;
                    break;
                }

                args_qty++;
                break;
            case IR_FUNC_INST_TYPE_CALL:
                LLVMValueRef func = LLVMGetNamedFunction(mod, ir_mod->vals.data[ir_mod->funcs.inst_data[i][j].idx].str);
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

    char* err_msg = NULL;
    if(LLVMVerifyModule(mod, LLVMReturnStatusAction, &err_msg))
    {
        printf("%s\n", err_msg);
        LLVMDisposeMessage(err_msg);
        LLVMDisposeBuilder(builder);
        LLVMDisposeModule(mod);
        LLVMContextDispose(ctx);
        return false;
    }

    if(!llvm_ir_optimization(mod)) return false;

    LLVMDumpModule(mod);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(mod);
    LLVMContextDispose(ctx);

    return true;
}