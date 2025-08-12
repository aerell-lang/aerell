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

#include "argp.h"

#include <stdio.h>
#include <stdlib.h>

void argp_short_options_init(int amount) {
    short_options_amount = amount;
    short_options_status = malloc(short_options_amount * sizeof(bool));
    short_options_char = malloc(short_options_amount * sizeof(char));
}

void argp_short_option_set(int index, char character) {
    short_options_status[index] = false;
    short_options_char[index] = character;
}

void argp_short_options_destroy() {
    free(short_options_status);
    free(short_options_char);
}

void argp(int argc, const char* argv[], bool verbose) {
    if(verbose) {
        printf("Argument parsing verbose...\n");
        printf("argc = %d\n", argc);
    }
    for(int i = 0; i < argc; ++i) {
        if(verbose) printf("argv[%d] = %s\n", i, argv[i]);
        if(argv[i][0] == '-') {
            for(int cfi = 0; cfi < short_options_amount; ++cfi) {
                if(argv[i][1] == short_options_char[cfi]) {
                    short_options_status[cfi] = true;
                }
            }
        }
    }
}

void argp_debug() {
    printf("Argument parsing debug...\n");
    printf("Char flag:\n");
    printf("amount = %d\n", short_options_amount);
    for(int cfi = 0; cfi < short_options_amount; ++cfi) {
        printf("%c = %i\n", short_options_char[cfi], short_options_status[cfi]);
    }
}