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

#include "lexer/token/token.h"
#include "lexer/token/tokens.h"
#include "parser/ast/asts.h"

ast_data_type_t* parseDataType(token_t** token_ref, tokens_t* tokens);

ast_func_param_t* parseFuncParam(token_t** token_ref, tokens_t* tokens);

asts_t* parseFuncParams(token_t** token_ref, tokens_t* tokens, bool* is_variadic_ref);

ast_func_t* parseFunc(token_t** token_ref, tokens_t* tokens);

asts_t* parser(tokens_t* tokens);

#endif