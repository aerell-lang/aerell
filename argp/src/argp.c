/*
 * Copyright (c) 2025, The Aerell Language Project Authors
 * All rights reserved.
 * 
 * This file is part of the Aerell Language project.
 * 
 * Licensed under the BSD 3-Clause License. 
 * You may obtain a copy of the License at:
 * https://github.com/aerell-lang/aerell/blob/main/LICENSE
 * 
 * File: argp.c
 * Description: Main source for the argp subproject.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#include "argp_short_option.h"
#include "argp.h"

#include <stdio.h>

void argp(int argc, const char* argv[], bool verbose) {
    if(verbose) {
        printf("Argument parsing verbose...\n");
        printf("argc = %d\n", argc);
    }
    for(int i = 0; i < argc; ++i) {
        if(verbose) printf("argv[%d] = %s\n", i, argv[i]);
        if(argv[i][0] == '-') {
            for(int cfi = 0; cfi < argp_short_options_amount; ++cfi) {
                if(argv[i][1] == argp_short_options_chars[cfi]) {
                    argp_short_options_status[cfi] = true;
                }
            }
        }
    }
}

void argp_debug() {
    printf("Argument parsing debug...\n");
    printf("Char flag:\n");
    printf("amount = %d\n", argp_short_options_amount);
    for(int cfi = 0; cfi < argp_short_options_amount; ++cfi) {
        printf("%c = %i\n", argp_short_options_chars[cfi], argp_short_options_status[cfi]);
    }
}