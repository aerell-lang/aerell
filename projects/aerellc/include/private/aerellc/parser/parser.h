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
 * Description: Parser function declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef PARSER_H
#define PARSER_H

#include "aerellc/lexer/token/token.h"
#include "aerellc/lexer/token/tokens.h"
#include "aerellc/parser/ast/asts.h"

ASTDataType* parseDataType(Token** token_ref, Tokens* tokens);

ASTFuncParam* parseFuncParam(Token** token_ref, Tokens* tokens);

ASTs* parseFuncParams(Token** token_ref, Tokens* tokens, bool* is_variadic_ref);

ASTFunc* parseFunc(Token** token_ref, Tokens* tokens);

ASTs* parser(Tokens* tokens);

#endif