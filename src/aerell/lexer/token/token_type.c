/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include "aerell/lexer/token/token_type.h"

const char* token_type_string[] = {
    // Special
    [TOKEN_TYPE_ILLEGAL] = "ILLEGAL",

    // Data types
    [TOKEN_TYPE_DT_BOOL] = "bool",
    [TOKEN_TYPE_DT_I8] = "i8",
    [TOKEN_TYPE_DT_I16] = "i16",
    [TOKEN_TYPE_DT_I32] = "i32",
    [TOKEN_TYPE_DT_I64] = "i64",
    [TOKEN_TYPE_DT_ISIZE] = "isize",
    [TOKEN_TYPE_DT_U8] = "u8",
    [TOKEN_TYPE_DT_U16] = "u16",
    [TOKEN_TYPE_DT_U32] = "u32",
    [TOKEN_TYPE_DT_U64] = "u64",
    [TOKEN_TYPE_DT_USIZE] = "usize",
    [TOKEN_TYPE_DT_F32] = "f32",
    [TOKEN_TYPE_DT_F64] = "f64",
    [TOKEN_TYPE_DT_CHAR] = "char",
    [TOKEN_TYPE_DT_STR] = "str",

    // Keywords
    [TOKEN_TYPE_KW_F] = "f",
    [TOKEN_TYPE_KW_VOID] = "void",
    [TOKEN_TYPE_KW_USE] = "use",
    [TOKEN_TYPE_KW_CONST] = "const",
    [TOKEN_TYPE_KW_STATIC] = "static",
    [TOKEN_TYPE_KW_GLOBAL] = "global",
    [TOKEN_TYPE_KW_VAR] = "var",
    [TOKEN_TYPE_KW_MUT] = "mut",

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