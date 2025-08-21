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
 * Description: Entry point for the aerell project.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#include <argp/argp.h>

int main(int argc, const char* argv[]) {
    ARGP* argp = argp_create(
        "aerell", 
        "1.0.0", 
        3
    );
    argp_set_option(argp, 0, "h", "help", "Print this usage information.");
    argp_set_option(argp, 1, "V", "version", "Print the aerell version.");
    argp_set_option(argp, 2, "v", "verbose", "Show verbose output.");
    argp_run(argp, argc, argv);
    if(argp_get_option(argp, 0)) {
        goto help;
    }else if(argp_get_option(argp, 1)) {
        argp_print_version(argp);
        goto exit;
    }
    help:
    argp_print_help(argp);
    exit:
    argp_free(argp);
    return 0;
}