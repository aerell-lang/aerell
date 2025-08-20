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
 * Description: Source for the argp feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#include "argp_short_option.h"
#include "argp_long_option.h"
#include "argp_option.h"
#include "argp.h"

#include <stdio.h>
#include <string.h>

void argp(int argc, const char* argv[]) {
    for(int i = 1; i < argc; ++i) {
        const char* sarg = argv[i];
        int sargc = strlen(sarg);
        if(sargc == 2 && sarg[0] == '-') {
            for(int j = 0; j < argp_option_amount; ++j) {
                if(sarg[1] == argp_short_option_chars[j]) {
                    argp_option_values[j] = true;
                    goto next;
                }
            }
        }
        if(sargc > 2 && sarg[0] == '-' && sarg[1] == '-') {
            int sargc1 = sargc - 2;
            for(int j = 0; j < argp_option_amount; ++j) {
                if(sargc1 == argp_long_option_char_sizes[j] && strncmp(sarg + 2, argp_long_option_chars[j], argp_long_option_char_sizes[j]) == 0) {
                    argp_option_values[j] = true;
                    goto next;
                }
            }
        }
        goto error;
        next:
            continue;
        error:
            printf("Unknown argument: '%s'\n", sarg);
            break;
    }
}