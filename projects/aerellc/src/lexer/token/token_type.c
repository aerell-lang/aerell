/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include "lexer/token/token_type.h"

const char* token_type_string[] = {
    // Special
    [TOKEN_TYPE_ILLEGAL] = "ILLEGAL",

    // Data types
    [TOKEN_TYPE_DT_I32] = "i32",
    [TOKEN_TYPE_DT_F32] = "f32",
    [TOKEN_TYPE_DT_STR] = "str",

    // Keywords
    [TOKEN_TYPE_KW_F] = "f",
    [TOKEN_TYPE_KW_VOID] = "void",
    [TOKEN_TYPE_KW_USE] = "use",
    [TOKEN_TYPE_KW_VAR] = "var",

    // Operators and delimiters
    [TOKEN_TYPE_ADD] = "+",
    [TOKEN_TYPE_ASSIGN] = "=",

    [TOKEN_TYPE_LPAREN] = "(",
    [TOKEN_TYPE_LBRACE] = "{",

    [TOKEN_TYPE_RPAREN] = ")",
    [TOKEN_TYPE_RBRACE] = "}",

    [TOKEN_TYPE_SEMICOLON] = ";",

    // Literal
    [TOKEN_TYPE_INT] = "INT",
    [TOKEN_TYPE_FLOAT] = "FLOAT",
    [TOKEN_TYPE_CHAR] = "CHAR",
    [TOKEN_TYPE_STRING] = "STRING",

    // Additional
    [TOKEN_TYPE_ID] = "ID",
};