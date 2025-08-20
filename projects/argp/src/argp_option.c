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
 * File: argp_option.c
 * Description: Source for option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-20
 */

#include "argp_option.h"

#include <stdlib.h>

int argp_option_amount = 0;
bool* argp_option_values = NULL;

void argp_options_init(int amount) {
    argp_option_amount = amount;
    argp_option_values = malloc(sizeof(bool) * argp_option_amount);
}

void argp_option_set(int index, bool status) {
    argp_option_values[index] = status;
}

bool argp_option_get(int index) {
    return argp_option_values[index];
}

void argp_options_destroy() {
    free(argp_option_values);
}