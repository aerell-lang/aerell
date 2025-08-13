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

int main(int argc, const char* argv[]) {
    argp_short_options_init(2);
    argp_short_option_set(0, 'h');
    argp_short_option_set(1, 'v');
    argp(argc, argv, true);
    argp_debug();
    argp_short_options_destroy();
    return 0;
}