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
 * File: parser.c
 * Description: Parser function definition.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>

#include "aerellc/lexer/token/token_type.h"
#include "aerellc/parser/parser.h"
#include "aerellc/parser/ast/ast.h"

ast_data_type_t* parseDataType(token_t** token_ref, tokens_t* tokens)
{
    // Validate parameter is valid
    if(!token_ref || !*token_ref || !tokens) return NULL;

    // Expect current token is data type
    token_type_t types[] = {
        TOKEN_DATA_TYPE_I1, TOKEN_DATA_TYPE_I8, TOKEN_DATA_TYPE_I16, TOKEN_DATA_TYPE_I32, TOKEN_DATA_TYPE_I64};
    if(!token_types_is(*token_ref, sizeof(types) / sizeof(types[0]), types))
    {
        printf("Expect data type i1, i8, i16, i32, i64.\n");
        return NULL;
    }

    // Save value
    token_t* value = *token_ref;

    // Next token
    *token_ref = tokens_get_token(tokens);

    // Check is pointer or not
    bool is_pointer = false;
    if(token_type_is(*token_ref, TOKEN_ASTERISK))
    {
        is_pointer = true;
        *token_ref = tokens_get_token(tokens);
    }

    return ast_data_type_create(value, is_pointer);
}

ast_func_param_t* parseFuncParam(token_t** token_ref, tokens_t* tokens)
{
    // Validate parameter is valid
    if(!token_ref || !*token_ref || !tokens) return NULL;

    // Expect current token is id
    if(!token_type_is(*token_ref, TOKEN_ID))
    {
        printf("Expect identifier for function parameter.\n");
        return NULL;
    }

    // Save name token
    token_t* name = *token_ref;

    // Next token
    *token_ref = tokens_get_token(tokens);

    // Save data type
    ast_data_type_t* data_type = parseDataType(token_ref, tokens);
    if(!data_type) return NULL;

    return ast_func_param_create(name, data_type);
}

asts_t* parseFuncParams(token_t** token_ref, tokens_t* tokens, bool* is_variadic_ref)
{
    // Validate parameter is valid
    if(!token_ref || !*token_ref || !tokens) return NULL;

    // Expect current token is open parentheses
    if(!token_type_is(*token_ref, TOKEN_OPEN_PARENTHESES))
    {
        printf("Expect '(' for function parameters.\n");
        return NULL;
    }

    // Next token
    *token_ref = tokens_get_token(tokens);

    asts_t* params = asts_create();

    while(!token_type_is(*token_ref, TOKEN_CLOSE_PARENTHESES))
    {
        // if variadic consume '...'
        if(token_type_is(*token_ref, TOKEN_VARIADIC))
        {
            *is_variadic_ref = true;
            *token_ref = tokens_get_token(tokens);
            break;
        }

        if(!asts_add(params, (ast_t*)parseFuncParam(token_ref, tokens)))
        {
            asts_free(params);
            return NULL;
        }

        // Next token if current token is coma and continue
        if(token_type_is(*token_ref, TOKEN_COMA))
        {
            *token_ref = tokens_get_token(tokens);

            // If current token after coma is ')' printf error
            if(token_type_is(*token_ref, TOKEN_CLOSE_PARENTHESES))
            {
                printf("Trailing comma in function parameters.\n");
                asts_free(params);
                return NULL;
            }

            continue;
        }

        break;
    }

    // Expect current token is close parentheses
    if(!token_type_is(*token_ref, TOKEN_CLOSE_PARENTHESES))
    {
        printf("Expect ')' for function parameters.\n");
        asts_free(params);
        return NULL;
    }

    // Consume ')'
    *token_ref = tokens_get_token(tokens);

    // Ret params
    return params;
}

ast_func_t* parseFunc(token_t** token_ref, tokens_t* tokens)
{
    // Validate parameter is valid
    if(!token_ref || !*token_ref || !tokens) return NULL;

    // Expect current token is f keyword
    if(!token_type_is(*token_ref, TOKEN_F))
    {
        printf("Expect 'f' keyword for function.\n");
        return NULL;
    }

    // Next token
    *token_ref = tokens_get_token(tokens);

    // Expect current token is id
    if(!token_type_is(*token_ref, TOKEN_ID))
    {
        printf("Expect identifier for function.\n");
        return NULL;
    }

    // Save name token
    token_t* name = *token_ref;

    // Next token
    *token_ref = tokens_get_token(tokens);

    // variadic
    bool is_variadic = false;

    // Parse params
    asts_t* params = parseFuncParams(token_ref, tokens, &is_variadic);
    if(!params) return NULL;

    // Return data type
    ast_data_type_t* return_data_type = parseDataType(token_ref, tokens);
    if(!return_data_type)
    {
        asts_free(params);
        return NULL;
    }

    // Expect current token is semicolon
    if(!token_type_is(*token_ref, TOKEN_SEMICOLON))
    {
        printf("Expect ';' for declaration of function.\n");
        asts_free(params);
        return NULL;
    }

    return ast_func_create(name, params, is_variadic, return_data_type);
}

asts_t* parser(tokens_t* tokens)
{
    asts_t* asts = asts_create();
    token_t* token = tokens_get_token(tokens);
    while(token != NULL && !token_type_is(token, TOKEN_EOF))
    {
        // Function
        if(token_type_is(token, TOKEN_F))
        {
            asts_add(asts, (ast_t*)parseFunc(&token, tokens));
            continue;
        }

        // Get next token
        token = tokens_get_token(tokens);
    }
    asts_shrink(asts);
    return asts;
}