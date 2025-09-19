/*
 * Copyright (c) 2025, The Aerell Language Project Authors
 * All rights reserved.
 *
 * This file is part of the Aerell Language project.
 *
 * Licensed under the BSD 3-Clause License.
 * You may obtain a copy of the License at:
 * https://github.com/aerell-lang/aerell/blob/main/LICENSE
 *
 * File: token_type.h
 * Description: Token type unique number declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-7
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