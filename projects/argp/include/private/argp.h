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
 * File: argp.h
 * Description: Private header for the argp feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#ifndef ARGP_H
#define ARGP_H

#include <stddef.h>

typedef struct ARGP {
    const char* name;
    const char* version;
    // Option
    size_t option_amount;
    bool* option_values;
    char* short_options;
    const char** long_options;
    size_t* long_option_sizes;
    const char** option_info;
    char** option_str_without_info;
    size_t option_str_without_info_max_size;
    // Command
    size_t command_amount;
    size_t* command_values;
    const char** commands;
    size_t* command_sizes;
    size_t* command_max_size;
    const char** command_info;
} ARGP;

ARGP* argp_create(const char* name, const char* version, size_t option_amount, size_t command_amount);
void argp_set_option(ARGP* argp, size_t index, char* short_version, const char* long_version, const char* info);
void argp_set_command(ARGP* argp, size_t index, const char* command, const char* info);
void argp_run(ARGP* argp, int argc, const char* argv[], size_t begin);
bool argp_get_option(ARGP* argp, size_t index);
size_t argp_get_command(ARGP* argp, size_t index);
void argp_print_help(ARGP* argp);
void argp_print_version(ARGP* argp);
void argp_free(ARGP* argp);

#endif