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
 * File: argp_long_option.h
 * Description: Private header for long option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-20
 */

#ifndef ARGP_LONG_OPTION_H
#define ARGP_LONG_OPTION_H

extern const char** argp_long_option_chars;
extern int* argp_long_option_char_sizes;

void argp_long_options_init();
void argp_long_option_set(int index, const char* chars);
void argp_long_options_destroy();

#endif