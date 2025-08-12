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
 * Description: Main header for the argp subproject.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#ifndef ARGP_H
#define ARGP_H

#include <stdio.h>

static int short_options_amount = 0;
static bool* short_options_status = NULL;
static char* short_options_char = NULL;

void argp_short_options_init(int amount);
void argp_short_option_set(int index, char character);
void argp_short_options_destroy();

void argp(int argc, const char* argv[], bool verbose);
void argp_debug();

#endif