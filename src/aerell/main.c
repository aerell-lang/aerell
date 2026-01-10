// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <file/file.h>

#include "aerell/lexer.h"
#include "aerell/ast/ast.h"
#include "aerell/parser.h"

int main(int argc, const char* argv[])
{
    if(argc == 1) return 0;

    bool isLex = false;
    bool isParse = false;
    if(argc > 2)
    {
        isLex = strcmp(argv[1], "lex") == 0;
        isParse = strcmp(argv[1], "parse") == 0;
    }

    if(argc == 3 && (isLex || isParse))
    {

        const char* path = argv[2];
        file_t* file = file_load(path);
        if(file == NULL)
        {
            printf("Failed to load file %s\n", path);
            return 1;
        }

        lexer_t lexer = {0};
        lexer_set_file(&lexer, file);

        if(isLex)
        {
            token_t* token = NULL;
            do
            {
                token = lexer_get_token(&lexer);
                debug_token(token);
            } while(token->type != TOKEN_TYPE_EOF);

            printf("\nLexing finished.");
            return 0;
        }

        parser_t parser;
        parser_init(&parser, &lexer);

        ast_t* ast = parser_parse(&parser);
        if(ast == NULL)
        {
            printf("Failed to parse file %s\n", path);
            file_unload(file);
            return 1;
        }

        debug_ast(ast);

        free(ast);
        file_unload(file);

        printf("\nParsing finished.");
        return 0;
    }

    printf("Invalid arguments or too few or too many.\n");
    return 1;
}