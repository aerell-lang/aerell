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
 * File: main.c
 * Description: Entry point for the aerell subproject.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#include <argp/argp.h>

#include <stdio.h>

int main(int argc, const char* argv[]) {
    argp_options_init(2);
    argp_option_set(0, false);
    argp_option_set(1, false);
    argp_short_options_init();
    argp_short_option_set(0, 'h');
    argp_short_option_set(1, 'v');
    argp_long_options_init();
    argp_long_option_set(0, "help");
    argp_long_option_set(1, "version");
    argp(argc, argv);
    if(argp_option_get(0)) {
        goto help;
    }
    if(argp_option_get(1)) {
        printf("1.0.0");
        goto exit;
    }
    help:
    printf("This is help message.");
    exit:
    argp_long_options_destroy();
    argp_short_options_destroy();
    argp_options_destroy();
    return 0;
}