// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include "aerell/token/token_tag.h"

const char* token_tag_to_c_str(token_tag_t tag)
{
    switch(tag)
    {
    case TOKEN_TAG_INVALID: return "invalid";
    case TOKEN_TAG_NUML: return "numl";
    case TOKEN_TAG_EOF: return "eof";
    }
    return "UNKNOWN";
}