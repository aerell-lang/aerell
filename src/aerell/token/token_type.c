// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/token/token_type.h"

const char* to_string(token_type_t t)
{
    switch(t)
    {
    case TOKEN_TYPE_EOF: return "EOF";
    case TOKEN_TYPE_INTL: return "INTL";
    case TOKEN_TYPE_ILLEGAL: return "ILLEGAL";
    }
    return "UNKNOWN";
}