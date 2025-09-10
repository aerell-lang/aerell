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
 * File: lexer.h
 * Description: Lexer function interface.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef struct Token Token;

typedef struct Tokens Tokens;

void tokens_print(Tokens* tokens);

void tokens_free(Tokens* tokens);

Tokens* lexer(FILE* file);

#endif