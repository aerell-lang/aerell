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

// System
#define TOKEN_UNKNOWN 0
#define TOKEN_EOF 1

// Symbol
#define TOKEN_COMA 2
#define TOKEN_ASTERISK 3
#define TOKEN_SEMICOLON 4
#define TOKEN_OPEN_PARENTHESES 5
#define TOKEN_CLOSE_PARENTHESES 6

// Unique
#define TOKEN_VARIADIC 7

// Data Type
#define TOKEN_DATA_TYPE_I1 8
#define TOKEN_DATA_TYPE_I8 9
#define TOKEN_DATA_TYPE_I16 10
#define TOKEN_DATA_TYPE_I32 11
#define TOKEN_DATA_TYPE_I64 12

// Value
#define TOKEN_VALUE_NUM 13
#define TOKEN_VALUE_STR 14

// Id
#define TOKEN_ID 15


#endif