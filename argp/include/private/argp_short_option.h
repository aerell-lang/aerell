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
 * File: argp_short_option.h
 * Description: Private header for short option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#ifndef ARGP_SHORT_OPTION_H
#define ARGP_SHORT_OPTION_H

extern int argp_short_options_amount;
extern bool* argp_short_options_status;
extern char* argp_short_options_chars;

void argp_short_options_init(int amount);
void argp_short_option_set(int index, char character);
void argp_short_options_destroy();

#endif