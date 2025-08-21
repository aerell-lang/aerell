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

#include "argp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ARGP* argp_create(const char* name, const char* version, size_t option_amount, size_t command_amount) {
    ARGP* argp = malloc(sizeof(ARGP));
    argp->name = name;
    argp->version = version;
    // Option
    argp->option_amount = option_amount;
    if(option_amount > 0) {
        argp->option_values = malloc(option_amount * sizeof(bool));
        argp->short_options = malloc(option_amount * sizeof(const char));
        argp->long_options = malloc(option_amount * sizeof(const char*));
        argp->long_option_sizes = malloc(option_amount * sizeof(size_t));
        argp->option_info = malloc(option_amount * sizeof(const char*));
        argp->option_str_without_info = malloc(option_amount * sizeof(char*));
    }else{
        argp->option_values = NULL;
        argp->short_options = NULL;
        argp->long_options = NULL;
        argp->long_option_sizes = NULL;
        argp->option_info = NULL;
        argp->option_str_without_info = NULL;
    }
    argp->option_str_without_info_max_size = 0;
    // Command
    argp->command_amount = command_amount;
    if(command_amount > 0) {
        argp->command_values = malloc(command_amount * sizeof(size_t));
        argp->commands = malloc(command_amount * sizeof(const char*));
        argp->command_sizes = malloc(command_amount * sizeof(size_t));
        argp->command_info = malloc(command_amount * sizeof(const char*));
    }else{
        argp->command_values = NULL;
        argp->commands = NULL;
        argp->command_sizes = NULL;
        argp->command_info = NULL;
    }
    argp->command_max_size = NULL;
    return argp;
}

void argp_set_option(ARGP* argp, size_t index, char* short_version, const char* long_version, const char* info) {
    argp->option_values[index] = false;
    argp->short_options[index] = short_version ? short_version[0] : '\0';
    argp->long_options[index] = long_version;
    argp->long_option_sizes[index] = long_version ? strlen(long_version) : 0;
    argp->option_info[index] = info;
    size_t size = 0;
    if(short_version && long_version) {
        size = snprintf(NULL, 0, "  -%c, --%s", short_version[0], long_version);
        argp->option_str_without_info[index] = malloc(size + 1);
        snprintf(argp->option_str_without_info[index], size + 1, "  -%c, --%s", short_version[0], long_version);
    }else if(short_version) {
        size = snprintf(NULL, 0, "  -%c", short_version[0]);
        argp->option_str_without_info[index] = malloc(size + 1);
        snprintf(argp->option_str_without_info[index], size + 1, "  -%c", short_version[0]);
    }else if(long_version) {
        size = snprintf(NULL, 0, "  --%s", long_version);
        argp->option_str_without_info[index] = malloc(size + 1);
        snprintf(argp->option_str_without_info[index], size + 1, "  --%s", long_version);
    }else {
        size = snprintf(NULL, 0, "");
        argp->option_str_without_info[index] = malloc(size + 1);
        snprintf(argp->option_str_without_info[index], size + 1, "");
    }
    if(size > argp->option_str_without_info_max_size) {
        argp->option_str_without_info_max_size = size;
    }
}

void argp_set_command(ARGP* argp, size_t index, const char* command, const char* info) {
    argp->command_values[index] = 0;
    argp->commands[index] = command;
    argp->command_info[index] = info;
    argp->command_sizes[index] = strlen(command);
    if(argp->command_max_size == NULL || argp->command_sizes[index] > *argp->command_max_size) {
        argp->command_max_size = &argp->command_sizes[index];
    }
}

void argp_run(ARGP* argp, int argc, const char* argv[], size_t begin) {
    if(argc == 1) return;
    for(size_t i = begin; i < argc; ++i) {
        const char* sarg = argv[i];
        size_t sargc = strlen(sarg);
        if(sargc == 2 && sarg[0] == '-') {
            for(size_t j = 0; j < argp->option_amount; ++j) {
                if(argp->short_options[j] && sarg[1] == argp->short_options[j]) {
                    argp->option_values[j] = true;
                    goto next;
                }
            }
        }else if(sargc > 2 && sarg[0] == '-' && sarg[1] == '-') {
            size_t sargc1 = sargc - 2;
            for(size_t j = 0; j < argp->option_amount; ++j) {
                if(argp->long_options[j] && sargc1 == argp->long_option_sizes[j] && strncmp(sarg + 2, argp->long_options[j], argp->long_option_sizes[j]) == 0) {
                    argp->option_values[j] = true;
                    goto next;
                }
            }
        }else {
            for(size_t j = 0; j < argp->command_amount; ++j) {
                if(argp->commands[j] && sargc == argp->command_sizes[j] && strncmp(sarg, argp->commands[j], argp->command_sizes[j]) == 0) {
                    argp->command_values[j] = i + 1;
                    goto end;
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
    end:
}

bool argp_get_option(ARGP* argp, size_t index) {
    return argp->option_values[index];
}

size_t argp_get_command(ARGP* argp, size_t index) {
    return argp->command_values[index];
}

void argp_print_help(ARGP* argp) {
    printf("Usage: %s", argp->name);
    if(argp->command_amount > 0) printf(" <command>");
    if(argp->option_amount > 0) {
        printf(" [arguments]");
        printf("\n\nGlobal options:");
        for(size_t i = 0; i < argp->option_amount; ++i) {
            printf("\n%-*s    %s", (int)argp->option_str_without_info_max_size, argp->option_str_without_info[i], argp->option_info[i] ? argp->option_info[i] : "");
        }
    }
    if(argp->command_amount > 0) {
        printf("\n\nAvailable commands:");
        for(size_t i = 0; i < argp->command_amount; ++i) {
            printf("\n  %-*s    %s", (int)*argp->command_max_size, argp->commands[i], argp->command_info[i]);
        }
    }
}

void argp_print_version(ARGP* argp) {
    printf("%s", argp->version);
}

void argp_free(ARGP* argp) {
    free(argp->long_option_sizes);
    free(argp->long_options);
    free(argp->short_options);
    free(argp->option_values);
    for(size_t i = 0; i < argp->option_amount; ++i) {
        free(argp->option_str_without_info[i]);
        argp->option_str_without_info[i] = NULL;
    }
    free(argp->option_str_without_info);
    free(argp->command_values);
    free(argp->command_info);
    free(argp->command_sizes);
    free(argp->commands);
    free(argp);
    argp->long_option_sizes = NULL;
    argp->long_options = NULL;
    argp->short_options = NULL;
    argp->option_values = NULL;
    argp->option_str_without_info = NULL;
    argp->option_str_without_info_max_size = 0;
    argp->command_values = NULL;
    argp->command_info = NULL;
    argp->command_sizes = NULL;
    argp->commands = NULL;
    argp->command_max_size = NULL;
    argp = NULL;
}