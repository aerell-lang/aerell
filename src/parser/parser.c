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
 * Description: Parser function implementation.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#include <stdio.h>
#include <string.h>

#include "parser/parser.h"
#include "lexer/token/token_type.h"

ASTs* parser(Tokens* tokens)
{
    printf("Parser is running.\n");
    ASTs* asts = asts_create();

    size_t i = 0;
    Token* token = tokens->data[i];
    while(token != NULL && token->type != TOKEN_TYPE_EOF)
    {
        if(token->type == TOKEN_TYPE_NEWLINE)
        {
            token = tokens->data[++i];
            continue;
        }

        if(token->type == TOKEN_TYPE_ID)
        {
            ASTId* id = ast_id_create(token);

            token = tokens->data[++i];

            if(token->type == TOKEN_TYPE_STR)
            {
                asts_add(id->children, (AST*)ast_str_create(token));
                token = tokens->data[++i];
            }

            asts_add(asts, (AST*)id);
            continue;
        }

        printf("Grammar error: '%s'.\n", token->content);
        token = tokens->data[++i];
    }

    if(!asts_shrink(asts))
    {
        printf("Failed to shrink asts.\n");
    }
    printf("Parser stopped.\n");
    return asts;
}