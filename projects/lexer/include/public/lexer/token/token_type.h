#pragma once

typedef enum
{
    END_OF_FILE,
    ILLEGAL,
    INTEGER,
    PRINT,
    IDENTIFIER,
} TokenType;

extern const char* TokenTypeNames[];