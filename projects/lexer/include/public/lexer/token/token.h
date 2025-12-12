#pragma once

#include "lexer/token/token_type.h"

typedef struct
{
    TokenType type;
    char* content;
} Token;