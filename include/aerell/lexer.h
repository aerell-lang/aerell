// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_LEXER_H
#define AERELL_LEXER_H

#include "aerell/token/token.h"

void lexer(const char* data, token_t* tokens, size_t size);

#endif