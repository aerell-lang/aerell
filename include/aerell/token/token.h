// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

#include "aerell/token/token_tag.h"
#include "aerell/token/token_loc.h"

typedef struct
{
    token_tag_t tag;
    token_loc_t loc;
} token_t;