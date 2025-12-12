#pragma once

#include <stddef.h>

#include "lexer/token/token.h"

typedef struct
{
    Token* data;
    size_t amount;
} Tokens;

Tokens* tokensCreate();

void tokensAdd(Tokens* tokens, Token token);

void tokensPrint(Tokens* tokens);

void tokensFree(Tokens* tokens);