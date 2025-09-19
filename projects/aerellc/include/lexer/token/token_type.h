/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

typedef enum token_type
{
    // System
    TOKEN_ILLEGAL,
    TOKEN_EOF,

    // Symbol
    TOKEN_COMA,
    TOKEN_ASTERISK,
    TOKEN_SEMICOLON,
    TOKEN_PARENT_OPEN,
    TOKEN_PARENT_CLOSE,

    // Keyword
    TOKEN_KW_F,

    // Unique
    TOKEN_VARIADIC,

    // Data Type
    TOKEN_DATA_TYPE_I1,
    TOKEN_DATA_TYPE_I8,
    TOKEN_DATA_TYPE_I16,
    TOKEN_DATA_TYPE_I32,
    TOKEN_DATA_TYPE_I64,

    // Value
    TOKEN_VALUE_NUM,
    TOKEN_VALUE_STR,

    // Id
    TOKEN_ID,
} token_type_t;

#endif