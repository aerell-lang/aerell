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
 * File: argp_short_option.c
 * Description: Source for short option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#include "argp_short_option.h"
#include "argp_option.h"

#include <stdlib.h>

char* argp_short_option_chars = NULL;

void argp_short_options_init() {
    argp_short_option_chars = malloc(argp_option_amount * sizeof(char));
}

void argp_short_option_set(int index, char character) {
    argp_short_option_chars[index] = character;
}

void argp_short_options_destroy() {
    free(argp_short_option_chars);
}