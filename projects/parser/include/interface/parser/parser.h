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
 * File: parser.h
 * Description: Parser and ast function interface.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef PARSER_H
#define PARSER_H

#include <lexer/lexer.h>

typedef struct ASTs ASTs;

ASTs* parser(Tokens* tokens);

void asts_print(ASTs* asts, size_t indent);

void asts_free(ASTs* asts);

#endif