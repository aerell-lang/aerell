#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/token/tokens.h"
#include "lexer/token/token.h"

Tokens* tokensCreate()
{
    Tokens* tokens = malloc(sizeof(Tokens));
    tokens->amount = 0;
    tokens->data = NULL;
    return tokens;
}

void tokensAdd(Tokens* tokens, Token token)
{
    Token* data = realloc(tokens->data, sizeof(Token) * (tokens->amount + 1));
    if(!data) return;
    tokens->data = data;
    tokens->data[tokens->amount] = token;
    tokens->amount++;
}

void tokensPrint(Tokens* tokens)
{
    size_t maxLen = 0;
    for(size_t i = 0; i < tokens->amount; i++)
    {
        size_t len = strlen(TokenTypeNames[tokens->data[i].type]);
        if(len > maxLen) maxLen = len;
    }

    for(size_t i = 0; i < tokens->amount; i++)
    {
        Token data = tokens->data[i];
        printf("%*s %s\n", (int)maxLen, TokenTypeNames[data.type], data.content == NULL ? "" : data.content);
    }
}

void tokensFree(Tokens* tokens)
{
    if(tokens == NULL) return;
    for(size_t i = 0; i < tokens->amount; i++)
    {
        char* content = tokens->data[i].content;
        if(content == NULL) continue;
        free(content);
    }
    free(tokens);
}