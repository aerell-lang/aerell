// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/file.h"
#include "aerell/token/tokenizer.h"
#include "aerell/ir/mod/ir_mod.h"

typedef struct
{
    file_t* file;
    ir_mod_t* mod;
    tokenizer_t tokenizer;
} ir_gen_t;

bool ir_gen_generate(ir_gen_t* self, file_t* file, ir_mod_t* mod);