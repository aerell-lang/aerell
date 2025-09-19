/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

typedef enum token_type
{
    // Special
    TOKEN_TYPE_ILLEGAL, // Ilegal or error

    // Keywords
    TOKEN_TYPE_KW_F, // f
    TOKEN_TYPE_KW_VOID, // void
    TOKEN_TYPE_KW_USE, // use
    TOKEN_TYPE_KW_VAR, // var

    // Data types
    TOKEN_TYPE_DT_I32, // i32
    TOKEN_TYPE_DT_F32, // f32
    TOKEN_TYPE_DT_STR, // str

    // Operators and delimiters
    TOKEN_TYPE_ADD, // +
    TOKEN_TYPE_ASSIGN, // =

    TOKEN_TYPE_LPAREN, // (
    TOKEN_TYPE_LBRACE, // {

    TOKEN_TYPE_RPAREN, // )
    TOKEN_TYPE_RBRACE, // }

    TOKEN_TYPE_SEMICOLON, // ;

    // Literal
    TOKEN_TYPE_INT, // 12345
    TOKEN_TYPE_FLOAT, // 123.45
    TOKEN_TYPE_CHAR, // 'a'
    TOKEN_TYPE_STRING, // "abc"

    // Additional
    TOKEN_TYPE_ID, // [_a-zA-Z][_a-zA-Z0-9]*

} token_type_t;

extern const char* token_type_string[];

#endif