// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aerell/parser.h"

bool parser_realloc_ast(parser_t* parser)
{
    size_t new_capacity = parser->ast_capacity == 0 ? 2 : parser->ast_capacity * 2;
    ast_t* ast = realloc(parser->ast, new_capacity * sizeof(ast_t));

    if(ast == NULL)
    {
        printf("Parser ast reallocation failed.\n");
        return false;
    }

    parser->ast = ast;
    parser->ast_capacity = new_capacity;

    return true;
}

bool parser_realloc_ast_if_require(parser_t* parser)
{
    if(parser->ast_size < parser->ast_capacity) return true;
    return parser_realloc_ast(parser);
}

void parser_init(parser_t* parser, lexer_t* lexer)
{
    assert(parser != NULL);
    assert(lexer != NULL);

    if(parser == NULL || lexer == NULL) return;

    parser->lexer = lexer;
    parser->ast = NULL;
    parser->ast_capacity = 2;
    parser->ast_size = 0;

    parser_realloc_ast(parser);
}

ast_t* parser_parse(parser_t* parser)
{
    assert(parser != NULL);
    assert(parser->ast != NULL);

    if(parser == NULL || parser->ast == NULL) return NULL;

    token_t* token = lexer_get_token(parser->lexer);
    while(token != NULL && token->type != TOKEN_TYPE_EOF)
    {
        if(token->type == TOKEN_TYPE_INTL)
        {
            if(!parser_realloc_ast_if_require(parser))
            {
                free(parser->ast);
                parser->ast_capacity = 0;
                parser->ast_size = 0;
                return NULL;
            }

            parser->ast[parser->ast_size].type = AST_TYPE_INTL;
            parser->ast[parser->ast_size].value = 0;

            char buffer[token->size + 1];
            strncpy(buffer, file_get_content(token->file) + token->offset, token->size);
            buffer[token->size] = '\0';

            char* endptr;

            parser->ast[parser->ast_size].value = (int)strtoll(buffer, &endptr, 10);

            if(buffer == endptr)
            {
                printf("Failed to convert %s to int.\n", buffer);
                token = lexer_get_token(parser->lexer);
                continue;
            }

            token = lexer_get_token(parser->lexer);
            parser->ast_size++;
            continue;
        }

        debug_token(token);
        token = lexer_get_token(parser->lexer);
    }

    if(!parser_realloc_ast_if_require(parser))
    {
        free(parser->ast);
        parser->ast_capacity = 0;
        parser->ast_size = 0;
        return NULL;
    }

    parser->ast[parser->ast_size].type = AST_TYPE_EOF;
    parser->ast[parser->ast_size].value = 0;

    return parser->ast;
}