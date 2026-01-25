// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <stdlib.h>
#include <stdio.h>

#include "aerell/file.h"
#include "aerell/ir/ir_gen.h"
#include "aerell/ir/mod/ir_mod.h"
#include "aerell/backend/c/c_emit.h"

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

    const char* buff = c_emit(&ir_mod);
    if(buff == NULL)
    {
        printf("Failed to emit c.\n");
        ir_mod_free(&ir_mod);
        file_unload(&file);
        return 1;
    }

    FILE* fw = fopen("a.c", "w");
    if(fw == NULL)
    {
        printf("Failed to write c.\n");
        ir_mod_free(&ir_mod);
        file_unload(&file);
        return 1;
    }

    // printf("%s", cmod);
    fprintf(fw, "%s", buff);

    printf("Emit a.c succesfully.\n");

    fclose(fw);
    free((char*)buff);
    ir_mod_free(&ir_mod);
    file_unload(&file);
    return 0;
}