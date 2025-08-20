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
 * File: argp_option.h
 * Description: Private header for option feature.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-20
 */

#ifndef ARGP_OPTION_H
#define ARGP_OPTION_H

extern int argp_option_amount;
extern bool* argp_option_values;

void argp_options_init(int amount);
void argp_option_set(int index, bool value);
bool argp_option_get(int index);
void argp_options_destroy();

#endif