#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("Require a file path to open");
        return EXIT_FAILURE;
    }

    argv++;

    FILE* file = fopen(*argv, "rb");
    if(file == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char input[fileSize + 1];

    fread(input, 1, fileSize, file);
    input[fileSize] = '\0';

    fclose(file);

    Tokens* tokens = lexer(input);

    tokensPrint(tokens);

    tokensFree(tokens);

    return EXIT_SUCCESS;
}