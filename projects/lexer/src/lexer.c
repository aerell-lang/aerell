#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/string.h"
#include "lexer/lexer.h"
#include "lexer/token/token_type.h"

typedef struct
{
    const TokenType type;
    const char* keyword;
} Keyword;

const Keyword keywords[] = {{.type = PRINT, .keyword = "print"}};

const size_t keywordSize = sizeof(keywords) / sizeof(keywords[0]);

bool isWhiteSpace(const char** input)
{
    if(!isspace(**input)) return false;
    (*input)++;
    return true;
}

bool isInteger(const char** input, Tokens* tokens)
{
    if(!isdigit(**input)) return false;

    const char* start = (*input);

    while(isdigit(**input)) (*input)++;

    const char* end = (*input);

    Token token;
    token.type = INTEGER;
    token.content = substring(start, end);

    if(token.content == NULL) return false;

    tokensAdd(tokens, token);

    return true;
}

bool isKeyword(const char** input, const TokenType type, const char* keyword, Tokens* tokens)
{
    const char* inputTest = *input;
    const char* keywordTest = keyword;

    while(*keywordTest != '\0')
    {
        if(*inputTest == '\0' || *inputTest != *keywordTest) return false;
        inputTest++;
        keywordTest++;
    }

    if(!isspace(*inputTest) && *inputTest != '\0') return false;

    (*input) += strlen(keyword);

    Token token;
    token.type = type;
    token.content = strdup(keyword);

    if(token.content == NULL) return false;

    tokensAdd(tokens, token);

    return true;
}

bool isKeywords(const char** input, Tokens* tokens)
{
    for(size_t i = 0; i < keywordSize; i++)
    {
        Keyword data = keywords[i];
        if(isKeyword(&(*input), data.type, data.keyword, tokens)) return true;
    }
    return false;
}

bool isIdentifier(const char** input, Tokens* tokens)
{
    if(!isalpha(**input) && **input != '_') return false;

    const char* start = (*input);

    while(isalpha(**input) || **input == '_') (*input)++;
    while(isalnum(**input) || **input == '_') (*input)++;

    const char* end = (*input);

    Token token;
    token.type = IDENTIFIER;
    token.content = substring(start, end);

    if(token.content == NULL) return false;

    tokensAdd(tokens, token);

    return true;
}

Tokens* lexer(const char* input)
{
    Tokens* tokens = tokensCreate();

    while(*input != '\0')
    {
        if(isWhiteSpace(&input)) continue;
        if(isInteger(&input, tokens)) continue;
        if(isKeywords(&input, tokens)) continue;
        if(isIdentifier(&input, tokens)) continue;

        Token token;
        token.type = ILLEGAL;
        token.content = strdupWithChar(*input);

        if(token.content != NULL) tokensAdd(tokens, token);

        input++;
    }

    Token token;
    token.type = END_OF_FILE;
    token.content = NULL;

    tokensAdd(tokens, token);

    return tokens;
}