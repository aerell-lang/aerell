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
 * File: argp_long_option.c
 * Description: Source for long option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-20
 */

#include "argp_long_option.h"
#include "argp_option.h"

#include <stdlib.h>
#include <string.h>

const char** argp_long_option_chars = NULL;
int* argp_long_option_char_sizes = NULL;

void argp_long_options_init() {
    argp_long_option_chars = malloc(argp_option_amount * sizeof(char*));
    argp_long_option_char_sizes = malloc(argp_option_amount * sizeof(int));
}

void argp_long_option_set(int index, const char* chars) {
    argp_long_option_chars[index] = chars;
    argp_long_option_char_sizes[index] = strlen(chars);
}

void argp_long_options_destroy() {
    free(argp_long_option_chars);
    free(argp_long_option_char_sizes);
}