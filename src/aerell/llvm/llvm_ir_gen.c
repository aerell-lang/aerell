// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>

#include <llvm-c/Core.h>

#include "aerell/llvm/llvm_ir_gen.h"

void llvm_ir_gen_generate(llvm_ir_gen_t* self, const ir_mod_t* ir_mod)
{
    (void)self;

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

        for(uint32_t j = 0; j < inst_qty; j++)
        {
            switch(ir_mod->funcs.inst_type[i][j])
            {
            case IR_FUNC_INST_TYPE_ARG: break;
            case IR_FUNC_INST_TYPE_CALL: break;
            }
        }

        LLVMBuildRetVoid(builder);
    }

    LLVMDumpModule(module);

    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
}