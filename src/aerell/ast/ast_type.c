// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ast/ast_type.h"

const char* ast_type_to_string(ast_type_t t)
{
    switch(t)
    {
    case AST_TYPE_EOF: return "EOF";
    case AST_TYPE_INTL: return "INTL";
    }
    return "UNKNOWN";
}