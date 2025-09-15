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
 * Description: Lexer function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
 */

#ifndef LEXER_H
#define LEXER_H

#include "aerellc/lexer/token/tokens.h"
#include "aerellc/source/source_file.h"

int lexer_add_token(tokens_t* tokens, int type, const char* content);

int lexer_add_token_with_char(tokens_t* tokens, int type, const char content);

tokens_t* lexer(SourceFile* source_file);

#endif