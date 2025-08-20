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
 * Description: Main interface header for the argp project.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-08-12
 */

#ifndef ARGP_H
#define ARGP_H

void argp_options_init(int amount);
void argp_option_set(int index, bool value);
bool argp_option_get(int index);
void argp_options_destroy();

void argp_short_options_init();
void argp_short_option_set(int index, char character);
void argp_short_options_destroy();

void argp_long_options_init();
void argp_long_option_set(int index, const char* chars);
void argp_long_options_destroy();

void argp(int argc, const char* argv[]);

#endif