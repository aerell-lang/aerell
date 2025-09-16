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
 * File: ast_type.h
 * Description: AST type number declaration.
 * Author: Fern Aerell fernaerell2020@gmail.com
 * Created: 2025-09-8
 */

#ifndef AST_TYPE_H
#define AST_TYPE_H

typedef enum ast_type
{
    AST_DATA_TYPE,
    AST_FUNC_PARAM,
    AST_FUNC,
} ast_type_t;

#endif