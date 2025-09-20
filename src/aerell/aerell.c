/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include <stdio.h>
#include <string.h>

#include "aerell/source/source_file.h"
#include "aerell/source/source_manager.h"
#include "aerell/lexer/lexer.h"

int file_is_exist(const char* file_path) { return (fopen(file_path, "r")) ? 1 : 0; }

int main(int argc, char* argv[])
{
    // Exit if not argument
    if(argc <= 1)
    {
        printf("No arguments.\n");
        return 1;
    }

    // Print version
    if(strcmp(argv[1], "version") == 0)
    {
        if(argc > 2)
        {
            printf("Too many argument just to print version.\n");
            return 1;
        }
        printf("1.0.0\n");
        return 0;
    }

    // Print help
    if(strcmp(argv[1], "help") == 0)
    {
        if(argc > 2)
        {
            printf("Too many argument just to print help.\n");
            return 1;
        }
        printf("Usage: aerell <command|file> [arguments]\n\n"
               "Global options:\n"
               "  -v, --verbose  Show verbose output.\n\n"
               "Available commands:\n"
               "  help      Print this usage information.\n"
               "  version   Print the Aerell version.\n");
        return 0;
    }

    source_manager_t* source_manager = source_manager_create();
    if(!source_manager)
    {
        printf("Failed to create source manager.\n");
        return -1;
    }

    for(size_t i = 1; i < (size_t)argc; i++)
    {
        const char* file_path = argv[i];
        if(!file_is_exist(file_path))
        {
            printf("Failed to opening file '%s'.\n", file_path);
            continue;
        }
        source_file_t* source_file = source_file_create(file_path);
        if(!source_file)
        {
            printf("Failed to create source file '%s'.\n", file_path);
            continue;
        }
        if(!source_manager_add(source_manager, source_file)) printf("Failed to add source file to source manager.\n");
    }

    if(!source_manager_shrink(source_manager))
    {
        printf("Failed to shrink source manager.\n");
        source_manager_free(source_manager);
        return -1;
    }

    for(size_t i = 0; i < source_manager->length; i++)
    {
        source_file_print(source_manager->files[i]);

        // Tokenizer
        tokens_t* tokens = lexer(source_manager->files[i]);

        // Print token
        printf("\n[Lexer] Tokenization Result:\n");
        tokens_print(tokens);

        // Clean up
        tokens_free(tokens);
    }

    source_manager_free(source_manager);

    return 0;
}