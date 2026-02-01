// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>

#include "aerell/file.h"
#include "aerell/ir/ir_gen.h"
#include "aerell/ir/mod/ir_mod.h"
#include "aerell/llvm/llvm_init.h"
#include "aerell/llvm/llvm_ir.h"
#include "aerell/llvm/llvm_ir_mod.h"
// #include "aerell/c/c_emit.h"

int main(int argc, const char* argv[])
{
    argc--;
    argv++;

    if(argc == 0) return 0;

    file_t file = {0};
    if(!file_load(&file, *argv))
    {
        printf("Failed to open file.\n");
        return 1;
    }

    ir_mod_t ir_mod = {0};
    ir_gen_t ir_gen = {0};

    if(!ir_gen_generate(&ir_gen, &file, &ir_mod))
    {
        printf("Failed to generate ir.\n");
        return 1;
    }

    // ir_mod_dump(&ir_mod);

    // LLVM
    llvm_init();

    llvm_ir_mod_t llvm_ir_mod = {0};
    llvm_ir_mod_init(&llvm_ir_mod, file.path);

    if(!llvm_ir_generate(&ir_mod))
    {
        printf("Failed to generate llvm ir.\n");
        return 1;
    }

    // C
    // const char* buff = c_emit(&ir_mod);
    // if(buff == NULL)
    // {
    //     printf("Failed to emit c.\n");
    //     ir_mod_free(&ir_mod);
    //     file_unload(&file);
    //     return 1;
    // }

    // FILE* fw = fopen("a.c", "w");
    // if(fw == NULL)
    // {
    //     printf("Failed to write c.\n");
    //     ir_mod_free(&ir_mod);
    //     file_unload(&file);
    //     return 1;
    // }

    // printf("%s", cmod);
    // fprintf(fw, "%s", buff);

    // printf("Emit a.c succesfully.\n");

    // fclose(fw);
    // free((char*)buff);

    llvm_ir_mod_free(&llvm_ir_mod);
    ir_mod_free(&ir_mod);
    file_unload(&file);
    return 0;
}