/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
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