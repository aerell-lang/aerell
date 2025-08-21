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

#include <stdio.h>

int main(int argc, const char* argv[]) {
    ARGP* argp = argp_create(
        "aerell", 
        "1.0.0", 
        1,
        3
    );
    argp_set_option(argp, 0, "v", "verbose", "Show verbose output.");
    argp_set_command(argp, 0, "help", "Print this usage information.");
    argp_set_command(argp, 1, "version", "Print the aerell version.");
    argp_set_command(argp, 2, "say", "Print something.");
    argp_run(argp, argc, argv, 1);
    size_t command_begin = 0;
    if(argp_get_command(argp, 0)) {
        argp_print_help(argp);
        argp_free(argp);
    }else if(argp_get_command(argp, 1)) {
        argp_print_version(argp);
        argp_free(argp);
    }else if((command_begin = argp_get_command(argp, 2))) {
        argp_free(argp);
        ARGP* argp2 = argp_create(
            "aerell say", 
            NULL, 
            1, 
            2
        );
        argp_set_option(argp2, 0, NULL, "capitalize", "Always capitalize the first letter of a word.");
        argp_set_command(argp2, 0, "helloworld", "Print Hello World!.");
        argp_set_command(argp2, 1, "author", "Print Fern Aerell.");
        argp_run(argp2, argc, argv, command_begin);
        bool capitalize = argp_get_option(argp2, 0);
        if(argp_get_command(argp2, 0)) {
            capitalize ? printf("Hello World!\n") : printf("hello world!\n");
        }else if(argp_get_command(argp2, 1)) {
            capitalize ? printf("Fern Aerell\n") : printf("fern aerell\n");
        }else {
            argp_print_help(argp2);
        }
        argp_free(argp2);
    }else{
        argp_print_help(argp);
        argp_free(argp);
    }
    return 0;
}